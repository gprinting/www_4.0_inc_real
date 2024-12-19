<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * 옵션 가격관련
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/06/27 엄준현 생성
 * 2018/04/17 이청산 수정
 *=============================================================================
 */
define("INC_PATH", $_SERVER["INC"]);

include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/common_dao/ProductCommonDAO.inc");
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

class OptPriceUtil {
    private $conn;
    private $dao;
    private $util;

    private $validate_flag = false;

    public function __construct($param = false) {
        $this->setData($param);
    }

    public function setData($param) {
        if (!$param) {
            return false;
        }

        if (empty($param["conn"])) {
            $cp = new ConnectionPool();
            $this->conn = $cp->getPooledConnection();
        } else {
            $this->conn = $param["conn"];
        }
        if (empty($param["dao"])) {
            $this->dao = new ProductCommonDAO();
        } else {
            $this->dao = $param["dao"];
        }
        if (empty($param["util"])) {
            $this->util = new CommonUtil();
        } else {
            $this->util = $param["util"];
        }

        if (!empty($param["validate_flag"])) {
            $this->validate_flag = $param["validate_flag"];
        }
    }

    /**
     * @brief 옵션 가격 계산
     *
     * @param $param = 옵션가격 계산을 위한 데이터
     *
     * @return 계산된 덩어리 수
     */
    public function calcOptPrice($param) {
        $sell_site      = $param["sell_site"];
        $cate_sortcode  = $param["cate_sortcode"];
        $name           = $param["option_name"];
        $mpcode         = $param["mpcode"];
        $amt            = $param["amt"];
        $sell_price     = $this->util->rmComma($param["sell_price"]);
        $paper_mpcode   = $param["paper_mpcode"];
        $paper_info     = $param["paper_info"];
        $expect_box     = $param["expect_box"];
        $affil          = $param["affil"];

        $temp = [];
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["name"]          = $name;
        $temp["basic_yn"]      = 'N';
        if ($this->validate_flag || $name !== "빠른생산요청") {
            $temp["mpcode"] = $mpcode;
        }

        // 카테고리와 옵션명으로 맵핑코드 검색
        $opt_info_rs = $this->dao->selectCateOptInfo($this->conn, $temp);

        $price = 0;

        if ($opt_info_rs->EOF) {
            return $price;
        }

        $out_flag = false;

        while ($opt_info_rs && !$opt_info_rs->EOF) {
            $fields = $opt_info_rs->fields;

            if (empty($name)) {
                $name = $fields["opt_name"];
            }

            $mpcode = $fields["mpcode"];
            $depth1 = $fields["depth1"];
            $depth2 = $fields["depth2"];
            $depth3 = $fields["depth3"];

            switch ($name) {
            case "당일판" :
                $price = $this->getDayBoardPrice($depth1);
                break;
            case "시안요청" :
                $price = $this->getDraftRequestPrice($depth1);
                break;
            case "빠른생산요청" :
                if (0 < $sell_price && $sell_price < 100001 &&
                        $depth1 === "판매가의 10%추가") {
                    //echo "1 : $sell_price";
                    $price  = $this->getQuickProductionPrice($sell_price);
                    $out_flag = true;
                } else if (100000 < $sell_price && $sell_price < 200001 &&
                        $depth1 === "판매가의 8%추가") {
                    //echo "2 : $sell_price";
                    $price  = $this->getQuickProductionPrice($sell_price);
                    $out_flag = true;
                } else if (200000 < $sell_price && $sell_price < 300001 &&
                        $depth1 === "판매가의 6%추가") {
                    //echo "3 : $sell_price";
                    $price  = $this->getQuickProductionPrice($sell_price);
                    $out_flag = true;
                } else if (300000 < $sell_price && $sell_price < 500001 &&
                        $depth1 === "판매가의 5%추가") {
                    //echo "4 : $sell_price";
                    $price  = $this->getQuickProductionPrice($sell_price);
                    $out_flag = true;
                } else if (500000 < $sell_price &&
                        $depth1 === "판매가의 3%추가") {
                    //echo "5 : $sell_price";
                    $price  = $this->getQuickProductionPrice($sell_price);
                    $out_flag = true;
                }
                break;
            case "정매생산요청" :
                if ($depth1 === "무료") {
                    $price = 0;
                    break;
                }

                unset($temp);
                $temp["cate_sortcode"]     = $cate_sortcode;
                $temp["sell_site"]         = $sell_site;
                $temp["paper_info"]        = $paper_info;
                $temp["cate_paper_mpcode"] = $paper_mpcode;
                $temp["affil"]             = $affil;
                $temp["amt"]               = $amt;
                $price = $this->getCorrectCountProductionPrice($temp);
                break;
            case "포장방법" :
                // 종이 분류에 따라서 일반용지는 500장 고급용지는 200장 기준
                // ->변경됨, 덩어리수로 계산
                // 170321 무료로 변경
                // 180413 유료로 변경
                //$chunk = $this->getAmtChunk($conn, $dao, $util, $fb->getForm());
                // 수량 콤마 제거
                $expect_box = $this->util->rmComma($expect_box);
                $price = $this->getPackPrice($depth2, $expect_box);
                //$price = 0;
                break;
            case "동판/목형관리" :
                $price = $this->getCopperWoodPrice($depth1);
                break;
            case "색견본참고" :
                $price = $this->getColorSamplePrice($depth1);
                break;
            case "교정디지털출력" :
                $price = $this->getCorrectionPrintPrice($depth1, $depth2);
                break;
            case "감리요청" :
                $price = $this->getInspectionRequestPrice($depth1);
                break;
            case "판비추가" :
                $price = $this->getAddPlatePrice($depth1);
                break;
            case "베다인쇄" :
                $price = $this->getBackgroundPrice($depth1);
                break;
            }

            if ($out_flag) {
                break;
            }

            $opt_info_rs->MoveNext();
        }

        $price = $this->util->ceilVal($price);

        return [
            "mpcode" => $mpcode,
            "sell_price"  => $price
        ];
    }

    /**
     * @brief 
     *
     * @param $conn = db connection
     * @param $dao  = dao 객체
     * @param $util = front util 객체
     * @param $fb   = 폼빈 객체
     *
     * @return 계산된 덩어리 수
     */
    private function getAmtChunk($conn, $dao, $util, $fb) {
        $amt           = $fb["amt"];
        $cate_sortcode = $fb["cate_sortcode"];

        $amt = intval($amt);

        $sortcode_arr = $util->getTMBCateSortcode($conn, $dao, $cate_sortcode);

        $sortcode_t = $sortcode_arr["sortcode_t"];
        $sortcode_m = $sortcode_arr["sortcode_m"];
        $sortcode_b = $sortcode_arr["sortcode_b"];

        if ($sortcode_m === "003001") {
            // 일반명함
            $amt = ceil($amt / 500);
        } else if ($sortcode_m === "003002") {
            // 고급명함
            $amt = ceil($amt / 200);
        } else if ($sortcode_m === "003003") {
            // 카드명함
            $amt = ceil($amt / 200);
        } else if ($sortcode_b === "008001001") {
            // 스페셜상품 - 복권
            $amt = ceil($amt / 500);
        } else if ($sortcode_b === "008002001") {
            // 스페셜상품 - 종이자석
            $amt = ceil($amt / 500);
        } else if ($sortcode_t === "005") {
            // 전단
            $amt  = intval($fb["sheet_count"]) / 4000;

            if ($amt < 1) {
                $amt = 1;
            }
        }

        return $amt;
    }

    /**
     * @brief 당일판 기본가격 반환
     *
     * @param $depth1 = 당일판 depth1
     *
     * @return 가격
     */
    private function getDayBoardPrice($depth1) {
        return 0;
    }

    /**
     * @brief 시안요청 기본가격 반환
     *
     * @param $depth1 = 시안요청 depth1
     *
     * @return 가격
     */
    private function getDraftRequestPrice($depth1) {
        return 0;
    }

    /**
     * @brief 빠른생산요청 가격 반환
     *
     * @param $price  = 판매금액
     *
     * @return 가격
     */
    private function getQuickProductionPrice($price) {
        $ret = 0;
        $price = intval($price);

        if (0 < $price && $price < 100001) {
            $ret = $price * 0.1;
        } else if (100000 < $price && $price < 200001) {
            $ret = $price * 0.08;
        } else if (200000 < $price && $price < 300001) {
            $ret = $price * 0.06;
        } else if (300000 < $price && $price < 500001) {
            $ret = $price * 0.05;
        } else if (500000 < $price) {
            $ret = $price * 0.03;
        }

        return $ret;
    }

    /**
     * @brief 정매생산요청 가격 반환
     
     * @param $param = 검색용 파라미터
     *
     * @return 가격
     */
    private function getCorrectCountProductionPrice($param) {
        // 기준수량 - R
        $PAPER_AMT = array(
            "아트지|46" => array(
                "90g" => array(
                    // 카테고리 수량(R) => 여분지 수량(장)
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 224
                ),
                "120g" => array(
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 225
                ),
                "150g" => array(
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 225
                ),
                "180g" => array(
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 225
                ),
                "300g" => array(
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 225
                )
            ),
            "아트지|국" => array(
                "90g" => array(
                    // 카테고리 수량(R) => 여분지 수량(장)
                    25  => 179,
                    50  => 224,
                    75  => 269,
                    100 => 314 
                ),
                "120g" => array(
                    25  => 179,
                    50  => 225,
                    75  => 270,
                    100 => 315
                ),
                "150g" => array(
                    25  => 180,
                    50  => 225,
                    75  => 270,
                    100 => 315
                ),
                "180g" => array(
                    25  => 180,
                    50  => 225,
                    75  => 270,
                    100 => 315
                )
            ),
            "모조지|46" => array(
                "80g" => array(
                    25  => 90,
                    50  => 135,
                    75  => 180,
                    100 => 225
                ),
            ),
            "모조지|국" => array(
                "80g" => array(
                    25  => 180,
                    50  => 225,
                    75  => 270,
                    100 => 315
                ),
            )
        );

        if ($param["cate_sortcode"] === "008001006") {
            $calc_price = 10000;
        } else {
            $sell_site         = $param["sell_site"];
            if (is_array($param["paper_info"])) {
                $paper_info_arr = $param["paper_info"];
            } else {
                $paper_info_arr = explode(' ', $param["paper_info"]);
            }
            $paper_name        = $paper_info_arr[0];
            $basisweight       = array_pop($paper_info_arr);
            $cate_paper_mpcode = $param["cate_paper_mpcode"];
            $affil             = $param["affil"];
            $amt               = intval($param["amt"]);

            /* TODO 가격관련 로직 필요 */
            if (empty($affil)) {
                $affil = "국";
            }
    
            $key = $paper_name . '|' . $affil;
    
            $extra_amt = 0.0;
            $extra_amt_arr = $PAPER_AMT[$key][$basisweight];
            foreach ($extra_amt_arr as $cate_amt => $extra_amt) {
                if ($amt <= $cate_amt) {
                    $extra_amt = doubleval($extra_amt);
                    break;
                }
            }
    
            unset($param);
            $param["mpcode"] = $cate_paper_mpcode;
            $param["affil"]  = $affil;
            $param["col"]    = "mpcode, crtr_unit";


            $prdt_paper_info = $this->dao->selectPrdtPaperInfo($this->conn, $param);
            $prdt_paper_mpcode = $prdt_paper_info["mpcode"];
            $paper_crtr_unit   = $prdt_paper_info["crtr_unit"];
    
    
            if ($paper_crtr_unit === 'R') {
                $extra_amt /= 500.0;
            }
    
            unset($param);
            $param["sell_site"] = $sell_site;
            $param["mpcode"]    = $prdt_paper_mpcode;
    
            $paper_price = $this->dao->selectPaperPrice($this->conn, $param);
            $calc_price  = $paper_price * $extra_amt;
    
            //echo $extra_amt . '/' . $paper_price;
        }

        return $calc_price;
    }

    /**
     * @brief 별도 포장 기본가격 반환
     *
     * @param $depth1 = 포장방법 depth2
     * @param $chunk  = 덩어리 수
     *
     * @return 가격
     */
    private function getPackPrice($depth2, $expect_box) {
        $ret = 0;
        switch ($depth2) {
        case "종이박스" :
            $ret = 0;
            break;
        case "명함케이스" :
            $ret = 500;
            break;
        case "종이(완포지)" :
            $ret = 0;
            break;
        case "박스" :
            $ret = 1000;
            break;
        case "지관통" :
            $ret = 2000;
            break;
        case "별도케이스" :
            $ret = 5000;
            break;
        case "종이(완포지)+박스" :
            $ret = 2000;
            break;
        }

        // 리턴 : 단가 * 배송 덩어리 수
        return $ret * $expect_box;
    }

    /**
     * @brief 동판/목형보관 기본가격 반환
     *
     * @param $depth1 = 동판/목형보관 depth1
     *
     * @return 가격
     */
    private function getCopperWoodPrice($depth1) {
        return 0;
    }

    /**
     * @brief 색견본 기본가격 반환
     *
     * @param $depth1 = 색견본 depth1
     *
     * @return 가격
     */
    private function getColorSamplePrice($depth1) {
        return 0;
    }

    /**
     * @brief 교정출력 기본가격 반환
     *
     * @param $depth1 = 교정출력 depth1
     * @param $depth2 = 교정출력 depth2
     *
     * @return 가격
     */
    private function getCorrectionPrintPrice($depth1, $depth2) {
        $ret = 0;

        switch ($depth1 . $depth2) {
        case "A4칼라" :
            $ret = 1000;
            break;
        case "A4흑백" :
            $ret = 500;
            break;
        case "A3칼라" :
            $ret = 2000;
            break;
        case "A3흑백" :
            $ret = 1000;
            break;
        }

        return $ret;
    }

    /**
     * @brief 감리요청 기본가격 반환
     *
     * @param $depth1 = 감리요청 depth1
     *
     * @return 가격
     */
    private function getInspectionRequestPrice($depth1) {
        return 50000;
    }

    /**
     * @brief 판비추가 기본가격 반환
     *
     * @param $depth1 = 판비추가 depth1
     *
     * @return 가격
     */
    private function getAddPlatePrice($depth1) {
        return 0;
    }

    /**
     * @brief 베다인쇄 기본가격 반환
     *
     * @param $depth1 = 베다인쇄 depth1
     *
     * @return 가격
     */
    private function getBackgroundPrice($depth1) {
        return 0;
    }
}
