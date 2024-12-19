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
 *=============================================================================
 */
define("INC_PATH", $_SERVER["INC"]);

include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/common_dao/ProductCommonDAO.inc");
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

class AftPriceUtil {
    private $conn;
    private $dao;
    private $util;

    // 박, 형압에서 사용
    private const FRONT     = 1; // 전면
    private const BACK      = 2; // 후면
    private const BOTH      = 3; // 양면같음
    private const BOTH_DIFF = 4; // 양면다름

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
    }

    /**
     * @brief 후공정 가격검색
     *
     * @param $param = 후공정 가격 데이터 검색용 파라미터
     *
     * @return 후공정 가격
     */
    public function getAfterPrice($param): int {
        $mpcode    = $param["mpcode"];
        $amt       = $param["amt"];

        $temp = array();
        $temp["mpcode"]    = $mpcode;
        $temp["amt"]       = $amt;

        $price_rs = $this->dao->selectCateAfterPrice($this->conn, $temp);

        $sell_price = $price_rs->fields["sell_price"];

        // 후공정 회원별 할인 추가 필요

        return intval($sell_price);
    }

    public function getAfterPrice1($param): int {
        $after_info_rs = $this->dao->selectCateAfterInfo($this->conn, $param);
        $mpcode_arr = $this->util->rs2arr($after_info_rs);
        $after_info_rs->MoveFirst();
        $crtr_unit_arr = $this->util->rs2arr($after_info_rs, "crtr_unit");

// 기준단위 배열 맵핑코드랑 매칭
        $mpcode_arr_count = count($mpcode_arr);

        $temp_arr = array();
        for ($i = 0; $i < $mpcode_arr_count; $i++) {
            $mpcode = $mpcode_arr[$i];
            $crtr_unit = $crtr_unit_arr[$i];

            $temp_arr[$mpcode] = $crtr_unit;
        }

        $crtr_unit_arr = $temp_arr;
        unset($temp_arr);
        unset($after_info_rs);
        //unset($param);

// 가격 검색
        //$mpcode = $this->util->arr2delimStr($mpcode_arr);

        //$param["mpcode"]    = $param[''];
        //$this->conn->debug = 1;
        $price_rs = $this->dao->selectCateAfterPrice($this->conn, $param);
        // 후공정 회원별 할인 추가 필요

        return intval($price_rs->fields["sell_price"]);
    }

    public function getAfterDepth($param) {
        $rs = $this->dao->selectAfterDepth($this->conn, $param);
        return $rs->fields;
    }

    public function getTheSizeMpcode($param) {
        $rs = $this->dao->selectTheSizeMpcode($this->conn, $param);
        return $rs->fields["mpcode"];
    }

    /**
     * @brief 박/형압/엠보싱 가격계산
     *
     * @param $param = 가격 계산용 파라미터
     *
     * @return 계산된 가격
     */
    public function getAfterFoilPressPrice($param) {
        $conn = $this->conn;
        $dao  = $this->dao;


        $success = true;
        $sum     = 0;
        $mpcode = '';
        $aft_name = '';
        $aft_depth = '';
        $aft_depth1 = '';
        $aft_depth1_dvs = '';
        $aft_vert = '';
        $aft_wid = '';

        $cate_sortcode = $param["cate_sortcode"];
        $aft_dvs       = $param["aft"];
        $amt           = intval($param["amt"]);
        $sheet_count   = intval($param["sheet_count"]);
        $size_name     = $param["size_name"];
        $aft_1         = $param["aft_1"];
        $dvs_1         = $param["dvs_1"];
        $cnt_1         = $param["cnt_1"];
        $aft_2         = $param["aft_2"];
        $dvs_2         = $param["dvs_2"];
        $aft_3         = $param["aft_3"];
        $dvs_3         = $param["dvs_3"];
        $wid_1         = intval($param["wid_1"]);
        $vert_1        = intval($param["vert_1"]);
        $wid_2         = intval($param["wid_2"]);
        $vert_2        = intval($param["vert_2"]);
        $wid_3         = intval($param["wid_3"]);
        $vert_3        = intval($param["vert_3"]);


        //print_r($param);

        $is_bl = false;
        if (substr($cate_sortcode, 0, 3) === "005") {
            $is_bl = true;

            if (!empty($sheet_count)) {
                $amt = $sheet_count;
            }
        }

        // 박인지 형압인지 구분
        // 박일 때 depth1 값을 기준으로 after_name 찾아야됨
        $temp = [];
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["amt"]           = $amt;
        $temp["size"]          = $size_name;

        if ($aft_dvs === "foil") {
            $dvs = "";
            $p1 = "";
            $p2 = "";
            $p3 = "";
/*
            if($aft_1) {
                $aft_depth1 = $aft_1;
                $aft_vert = $vert_1;
                $aft_wid = $wid_1;

                if ($dvs_1 === "양면") {
                    $dvs = self::BOTH;
                } else if ($dvs_1 === "전면") {
                    $dvs = self::FRONT;
                } else if ($dvs_1 === "후면") {
                    $dvs = self::BACK;
                }
                if ($wid_1 < 20) {
                    $wid_1  = 20;
                }
                if ($vert_1 < 20) {
                    $vert_1 = 20;
                }

                $temp["after_name"] = '박';
                $aft_depth = "박";
                $aft_name = 'foil1';
                $aft_depth1_dvs = $dvs_1;

                // 맵핑코드 검색용
                switch ($dvs) {
                    case self::FRONT :
                        $temp["depth2"] = "전면";
                        break;
                    case self::BACK :
                        $temp["depth2"] = "후면";
                        break;
                    case self::BOTH :
                        $temp["depth2"] = "양면";
                        break;
                }

                //$conn->debug = 1;
                $mpcode = '';

                $temp["depth1"] = $aft_1;
                $mpcode_1 = $dao->selectCateAfterInfo($conn, $temp)
                    ->fields["mpcode"];
                $mpcode = $mpcode_1;

                $sum = 0;
                if ($dvs === self::BOTH) {
                    // 양면같음
                    $temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = "양면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1 + $wid_1 + $vert_1;
                } else if ($dvs === self::FRONT) {
                    // 전면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1;
                } else if ($dvs === self::BACK) {
                    // 후면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1;
                } else {
                    $success = false;
                }
                $p1 = $sum;
            }

            if($aft_2) {
                $aft_depth1 .= "|" . $aft_2;
                $aft_vert .= "|" . $vert_2;
                $aft_wid .= "|" . $wid_2;

                if ($dvs_2 === "양면다름") {
                    $dvs = self::BOTH;
                } else if ($dvs_2 === "단면") {
                    $dvs = self::FRONT;
                } else if ($dvs_2 === "양면") {
                    $dvs = self::BACK;
                }
                if ($wid_2 < 20) {
                    $wid_2  = 20;
                }
                if ($vert_2 < 20) {
                    $vert_2 = 20;
                }

                $temp["after_name"] = '박';
                $aft_depth .= "|박";
                $aft_name .= '|foil2';

                // 맵핑코드 검색용
                switch ($dvs) {
                    case self::FRONT :
                        $temp["depth2"] = "단면";
                        break;
                    case self::BACK :
                        $temp["depth2"] = "양면";
                        break;
                    case self::BOTH :
                        $temp["depth2"] = "양면다름";
                        break;
                }

                $aft_depth1_dvs .= "|" . $dvs_2;
                $temp["depth1"] = $aft_2;
                $mpcode_2 = $dao->selectCateAfterInfo($conn, $temp)
                    ->fields["mpcode"];
                $mpcode .= '|' . $mpcode_2;

                $sum2 = 0;
                if ($dvs === self::BOTH) {
                    // 양면같음
                    $temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "양면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum2 = $price + $wid_2 + $vert_2 + $wid_2 + $vert_2;
                } else if ($dvs === self::FRONT) {
                    // 전면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum2 = $price + $wid_2 + $vert_2;
                } else if ($dvs === self::BACK) {
                    // 후면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum2 = $price + $wid_2 + $vert_2;
                } else {
                    $success = false;
                }
                $p2 = $sum2;
                $sum = $sum + $sum2;
            }

            if($aft_3) {
                $aft_depth1 .= "|" . $aft_3;
                $aft_vert .= "|" . $vert_3;
                $aft_wid .= "|" . $wid_3;

                if ($dvs_3 === "양면다름") {
                    $dvs = self::BOTH;
                } else if ($dvs_3 === "단면") {
                    $dvs = self::FRONT;
                } else if ($dvs_3 === "양면") {
                    $dvs = self::BACK;
                }
                if ($wid_3 < 20) {
                    $wid_3  = 20;
                }
                if ($vert_3 < 20) {
                    $vert_3 = 20;
                }
                $temp["after_name"] = '박';
                $aft_depth .= "|박";
                $aft_name .= '|foil3';

                // 맵핑코드 검색용
                switch ($dvs) {
                    case self::FRONT :
                        $temp["depth2"] = "단면";
                        break;
                    case self::BACK :
                        $temp["depth2"] = "양면";
                        break;
                    case self::BOTH :
                        $temp["depth2"] = "양면다름";
                        break;
                }

                $aft_depth1_dvs .= "|" . $dvs_3;
                $temp["depth1"] = $aft_3;
                $mpcode_3 = $dao->selectCateAfterInfo($conn, $temp)
                    ->fields["mpcode"];
                $mpcode .= '|' . $mpcode_3;

                $sum3 = 0;
                if ($dvs === self::BOTH) {
                    // 양면같음
                    $temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "양면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum3 = $price + $wid_3 + $vert_3 + $wid_3 + $vert_3;
                } else if ($dvs === self::FRONT) {
                    // 전면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum3 = $price + $wid_3 + $vert_3;
                } else if ($dvs === self::BACK) {
                    // 후면만
                    $temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum3 = $price + $wid_3 + $vert_3;
                } else {
                    $success = false;
                }
                $p3 = $sum3;
                $sum = $sum + $sum3;
            }
*/
            //echo $aft_1 . " " . $dvs_1 . " " . $aft_2 . " " . $dvs_2;exit;

            //$dvs = $this->chkFoilPressDvs($aft_1, $dvs_1, $aft_2, $dvs_2,$aft_3, $dvs_3);

        /*    if ($wid_1 < 20) {
                $wid_1  = 20;
            }
            if ($vert_1 < 20) {
                $vert_1 = 20;
            }
            
            if ($wid_2 < 20) {
                $wid_2  = 20;
            }
            if ($vert_2 < 20) {
                $vert_2 = 20;
            }
            if ($wid_3 < 20) {
                $wid_3  = 20;
            }
            if ($vert_3 < 20) {
                $vert_3 = 20;
            }
        */
            $temp["after_name"] = '박';
            $mpcode = '';
            if (!empty($aft_1)) {
                $temp["depth1"] = $aft_1;
                $temp["depth2"] = $dvs_1;

                $mpcode_1 = $dao->selectCateAfterInfo($conn, $temp)
                                ->fields["mpcode"];
                $aft_depth1 .= $aft_1;
                $aft_vert .= $vert_1;
                $aft_wid .=$wid_1;
                $aft_depth .= "박";
                $aft_name .= 'foil1';
                $aft_depth1_dvs .= $dvs_1;
                $mpcode .= $mpcode_1;
                $sum = 0;
                if ($dvs_1 === "양면" || $dvs_1 === "양면동일") {
                    // 양면같음
                    //$temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = $dvs_1;
                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);
                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1 + $wid_1 + $vert_1;
                } else if ($dvs_1 === "단면") {
                    // 전면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);
                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1;
                } else if ($dvs_1 === "양면다름") {
                    // 후면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_1);
                    $temp["dvs"]        = "양면다름";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
                    $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

                    $sum = $price + $wid_1 + $vert_1 + $wid_1 + $vert_1;
                } else {
                    $success = false;
                }
                $p1 = $sum;
            }
            if (!empty($aft_2)) {
                $temp["depth1"] = $aft_2;
                $temp["depth2"] = $dvs_2;
                $mpcode_2 = $dao->selectCateAfterInfo($conn, $temp)
                                ->fields["mpcode"];

                $aft_depth1 .= "|" . $aft_2;
                $aft_vert .= "|" . $vert_2;
                $aft_wid .= "|" . $wid_2;
                $aft_depth .= "|박";
                $aft_name .= '|foil2';
                $aft_depth1_dvs .= "|" . $dvs_2;
                $mpcode .= '|' . $mpcode_2;

                $sum2 = 0;
                if ($dvs_2 === "양면") {
                    // 양면같음
                    //$temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "양면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum = $price + $wid_2 + $vert_2 + $wid_2 + $vert_2;
                } else if ($dvs_2 === "단면") {
                    // 전면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum = $price + $wid_2 + $vert_2;
                } else if ($dvs_1 === "양면다름") {
                    // 후면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_2);
                    $temp["dvs"]        = "양면다름";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_2  = $this->calcFoilPressAreaVal($wid_2, $amt, $is_bl);
                    $vert_2 = $this->calcFoilPressAreaVal($vert_2, $amt, $is_bl);

                    $sum = $price + $wid_2 + $vert_2 + $wid_2 + $vert_2;
                } else {
                    $success = false;
                }
                $p2 = $sum2;
                $sum = $sum + $sum2;
            }

            if (!empty($aft_3)) {
                $temp["depth1"] = $aft_3;
                $temp["depth2"] = $dvs_3;
                $mpcode_3 = $dao->selectCateAfterInfo($conn, $temp)
                    ->fields["mpcode"];

                $aft_depth1 .= "|" . $aft_3;
                $aft_vert .= "|" . $vert_3;
                $aft_wid .= "|" . $wid_3;
                $aft_depth .= "|박";
                $aft_name .= '|foil3';
                $aft_depth1_dvs .= "|" . $dvs_3;
                $mpcode .= '|' . $mpcode_3;

                $sum3 = 0;
                if ($dvs_3 === "양면") {
                    // 양면같음
                    //$temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "양면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum = $price + $wid_3 + $vert_3 + $wid_3 + $vert_3;
                } else if ($dvs_3 === "단면") {
                    // 전면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "단면";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum = $price + $wid_3 + $vert_3;
                } else if ($dvs_3 === "양면다름") {
                    // 후면만
                    //$temp["after_name"] = $this->getFoilAfterName($aft_3);
                    $temp["dvs"]        = "양면다름";

                    $price = $dao->selectAfterFoilPressPrice($conn, $temp);

                    $wid_3  = $this->calcFoilPressAreaVal($wid_3, $amt, $is_bl);
                    $vert_3 = $this->calcFoilPressAreaVal($vert_3, $amt, $is_bl);

                    $sum = $price + $wid_3 + $vert_3 + $wid_3 + $vert_3;
                } else {
                    $success = false;
                }
                $p3 = $sum3;
                $sum = $sum + $sum3;
            }
        } else if($aft_dvs === "press") {
            if ($wid_1 < 20) {
                $wid_1  = 20;
            }
            if ($vert_1 < 20) {
                $vert_1 = 20;
            }

            $temp["after_name"] = "형압";
            $temp["depth1"] = $aft_1;
            $temp["depth2"] = "-";
            $temp["dvs"]        = "-";

            $mpcode_1 = $dao->selectCateAfterInfo($conn, $temp)
                ->fields["mpcode"];
            $aft_depth1 .= $aft_1;
            $aft_vert .= $vert_1;
            $aft_wid .=$wid_1;
            $aft_depth .= "형압";
            $aft_name .= 'press';
            $aft_depth1_dvs .= $temp["dvs"];
            $mpcode .= $mpcode_1;

            $price = $dao->selectAfterFoilPressPrice($conn, $temp);

            $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
            $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);

            $sum = $price + $wid_1 + $vert_1;
        } else if($aft_dvs === "embossing") {
            if ($wid_1 < 20) {
                $wid_1  = 20;
            }
            if ($vert_1 < 20) {
                $vert_1 = 20;
            }

            $temp["after_name"] = "엠보싱";
            $temp["depth1"] = $aft_1;
            $temp["depth2"] = $dvs_1;
            $temp["dvs"] = $dvs_1;
            $temp["size_name"] = $size_name;
            $temp["amt"] = $amt;
            $mpcode_1 = $dao->selectCateAfterInfo($conn, $temp)
                ->fields["mpcode"];
            //$temp["after_name"] = $aft_1;
            $aft_depth1 .= $aft_1;
            $aft_vert .= $vert_1;
            $aft_wid .=$wid_1;
            $aft_depth .= "엠보싱";
            $aft_name .= 'embossing';
            $aft_depth1_dvs .= $temp["dvs"];
            $mpcode .= $mpcode_1;
            $price = $dao->selectAfterEmbossingPrice($conn, $temp);
            $wid_1  = $this->calcFoilPressAreaVal($wid_1, $amt, $is_bl);
            $vert_1 = $this->calcFoilPressAreaVal($vert_1, $amt, $is_bl);
            $multi = $amt / $price["amt"] * 0.85;
            if($multi <= 1) $multi = 1;
            $sum = $price["price"] * $multi + $wid_1 + $vert_1;
        } else if($aft_dvs === "thomson") {
            if ($wid_1 < 20) {
                $wid_1  = 20;
            }
            if ($vert_1 < 20) {
                $vert_1 = 20;
            }

            $temp["after_name"] = "도무송";
            $temp["depth1"] = "자유형도무송";
            $temp["depth2"] = $dvs_1;
            $temp["dvs"] = $dvs_1;
            $temp["stan_mpcode"] = $param['stan_mpcode'];
            $temp["size_name"] = $dao->selectSizeName($conn, $temp);
            $mpcode_1 = $dao->selectCateAfterInfo($conn, $temp)
                ->fields["mpcode"];
            $aft_depth1 .= $aft_1;
            $aft_vert .= $vert_1;
            $aft_wid .=$wid_1;
            $aft_depth .= "도무송";
            $aft_name .= 'thomson';
            $aft_depth1_dvs .= $temp["dvs"];
            $mpcode .= $mpcode_1;
            $price = $dao->selectAfterFoilPressPrice($conn, $temp);
            $size = $wid_1 * $vert_1;
            if($size < 5000)
                $size = 5000;

            $price += $size;
            if($cnt_1 == 1) {
                $price *= 0.8;
            } else if($cnt_1 == 2) {
                $price *= 1;
            } else if($cnt_1 == 3) {
                $price *= 1.1;
            } else if($cnt_1 == 4) {
                $price *= 1.2;
            } else if($cnt_1 == 5) {
                $price *= 1.3;
            } else if($cnt_1 == 6) {
                $price *= 1.4;
            }
            //$price *= log($cnt_1) / log(10000);
            $price = ceil($price / 100) * 100;
            $sum = $price;
        }
        return [
            "success" => $success,
            "p1"   => $p1,
            "p2"   => $p2,
            "p3"   => $p3,
            "aft_depth1"   => $aft_depth1,
            "aft_vert"   => $aft_vert,
            "aft_wid"   => $aft_wid,
            "aft_depth"   => $aft_depth,
            "aft_name"   => $aft_name,
            "aft_depth1_dvs"   => $aft_depth1_dvs,
            "price"   => $sum,
            "mpcode"  => $mpcode
        ];
    }

    /**
     * @brief 박일 때 금박유광 이런식으로 넘어오는 이름을 금박만 반환
     *
     * @param $aft = 넘어온 후공정명
     *
     * @return 잘라낸 후공정명
     */
    private function getFoilAfterName($aft): string {
        /*
        if (strpos($aft, "금박") !== false) {
            return "금박";
        } else if (strpos($aft, "녹박") !== false) {
            return "녹박";
        } else if (strpos($aft, "먹박") !== false) {
            return "먹박";
        } else if (strpos($aft, "은박") !== false) {
            return "은박";
        } else if (strpos($aft, "적박") !== false) {
            return "적박";
        } else if (strpos($aft, "청박") !== false) {
            return "청박";
        } else {
            return $aft;
        }*/
        return $aft;
    }

    /**
     * @brief 양면/전면/후면/양면다름 판단
     *
     * @param $aft1 = 앞부분 박 선택값
     * @param $dvs1 = 앞부분 구분 선택값
     * @param $aft2 = 뒷부분 박 선택값
     * @param $dvs2 = 뒷부분 구분 선택값
     *
     * @return 구분값
     */
    private function chkFoilPressDvs($aft_1, $dvs_1, $aft_2, $dvs_2): int {
        if ($dvs_1 === "양면") {
            return self::BOTH;
        } else if (!empty($aft_1) && !empty($dvs_1) &&
            empty($aft_2) && empty($dvs_2)) {
            return self::FRONT;
        } else if (empty($aft_1) && empty($dvs_1) &&
            !empty($aft_2) && !empty($dvs_2)) {
            return self::BACK;
        } else if (!empty($aft_1) && !empty($dvs_1) &&
            !empty($aft_2) && !empty($dvs_2)) {
            if ($aft_1 === $aft_2) {
                return self::BOTH;
            }

            return self::BOTH_DIFF;
        }
    }

    /**
     * @brief 각 너비 가중값 계산
     *
     * @param $val   = 너비/높이값
     * @param $amt   = 수량
     * @param $is_bl = 전단이면 true / 아니면 false
     *
     * @return 계산값
     */
    private function calcFoilPressAreaVal($val, $amt, $is_bl) {
        $weight = 10;
        if ($is_bl) {
            $weight = 0.7;
        }

        $re_val = ($val / 10) - 2;
        if($re_val < 1) $re_val = 1;
        return $re_val * $weight * $amt;
    }

    /**
     * @brief 자유형 도무송 가격 계산
     *
     * @param $param = 가격 계산용 정보배열
     */
    public function getFreeTomsonPrice($param) {
        $conn = $this->conn;
        $dao  = $this->dao;

        // $Pp_id -> 종이관련 정보
        $paper_name = $param["paper_name"];
        // $S_Code -> 사이즈 유형 정보
        $stan_name  = $param["stan_name"];
        // $Qty -> 수량
        $amt        = intval($param["amt"]);
        // 도무송 수량
        $amt_ts     = intval($param["amt_ts"]);
        // $chkW, chkH
        $size_width = doubleval($param["size_width"]);
        $size_vert  = doubleval($param["size_vert"]);

        $amt_form = [intval($param["amt_f1"]), intval($param["amt_f2"])
            ,intval($param["amt_f3"]), intval($param["amt_f4"])];

        $temp = [];
        $temp["amt"] = $amt;
        //$conn->debug = 1;
        $price_per = $dao->selectAfterTomsonPricePer($conn, $temp);

        $basic_price = doubleval($price_per["basic_price"]);
        $knife_per   = doubleval($price_per["knife_price_per"]);
        $paper_per   = doubleval($price_per["stick_paper_price_per"]);

        if (strpos($paper_name, "아트") === false
            && strpos($paper_name, "모조") === false
            && strpos($paper_name, "크라프트") === false) {
            $paper_per = doubleval($price_per["especial_paper_price_per"]);
        }

        if (strpos($paper_name, "크라프트") !== false
            || strpos($paper_name, "모조") !== false) {
            $paper_per += 0.02;
        }

        //echo "b_p : $basic_price / k_p : $knife_per / p_p : $paper_per\n";

        $area = (($size_width + 5.0) * ($size_vert + 5.0)) / 100.0;
        $area = round($area, 2);

        if ($area < 30.0) {
            $area = 30.0;
        }

        //echo "area : $area\n";

        $price_1 = $area * $paper_per * $amt;

        //echo "price_1 : $price_1\n";

        if ($price_1 < $basic_price) {
            $price_1 = $basic_price;
        }

        //echo "price_1 : $price_1\n";

        $col_name = null;
        $stan_name = explode('[', $stan_name)[0];

        switch ($stan_name) {
            case "유형1":
                $col_name = "typ1_price";
                break;
            case "유형2":
                $col_name = "typ2_price";
                break;
            case "유형3":
                $col_name = "typ3_price";
                break;
            case "유형4":
                $col_name = "typ4_price";
                break;
            case "혼합유형":
                $col_name = ["typ1_price", "typ2_price", "typ3_price", "typ4_price"];
                break;
        }

        //echo "col_name : $col_name\n";
        $price_2 = 0;
        $typ_price = 0;

        $chk_area = $size_width * $size_vert / 100.0;
        $chk_area = round($chk_area, 1);

        $temp["size_start"] = $chk_area;
        $temp["size_end"]   = $chk_area;

        //$conn->debug = 1;

        if ($stan_name == "혼합유형") {
            for ($i = 0; $i < 4; $i++) {
                $temp["col_name"] = $col_name[$i];
                $price_fields = $dao->selectAfterTomsonPrice($conn, $temp);

                $basic_price = doubleval($price_fields["basic_price"]);
                $typ_price  += doubleval($price_fields[$col_name[$i]]) * $amt_form[$i];
            }
        } else {
            $temp["col_name"] = $col_name;
            $price_fields = $dao->selectAfterTomsonPrice($conn, $temp);

            $basic_price = doubleval($price_fields["basic_price"]);
            $typ_price   = doubleval($price_fields[$col_name] * $amt_ts);
        }


        //echo "basic_price : $basic_price / typ_price : $typ_price\n";

        $price_2 += ($basic_price + $typ_price) * $knife_per * $amt / 1000;

        //echo "price2 : $price_2\n";

        $calc_price = round($price_1 + $price_2, -2) * 1.1;

        //echo "calc_price : $calc_price\n";

        return $this->util->ceilVal($calc_price);
    }

    public function getFreeTomsonPrice_temp($param) {
        $conn = $this->conn;
        $dao  = $this->dao;

        // $Pp_id -> 종이관련 정보
        $paper_name = $param["paper_name"];
        // $S_Code -> 사이즈 유형 정보
        $stan_name  = $param["stan_name"];
        // $Qty -> 수량
        $amt        = intval($param["amt"]);
        // 도무송 수량
        $amt_ts     = intval($param["amt_ts"]);
        // $chkW, chkH
        $size_width = doubleval($param["size_width"]);
        $size_vert  = doubleval($param["size_vert"]);

        // 사이즈 - 773 x 520
        // 판값 - 1mm 판값 x 사이즈
        // 인쇄비 - 1mm 인쇄비 x 사이즈 x 매수
        // 용지비 - 1mm 용지비 x 사이즈 x 매수
        // 도무송칼값 - 1mm 도무송 칼값 x 사이즈 (7만원)
        // 도무송비 - 1mm 도무송 비 x 사이즈 x 매수 (5만원)

        // 판 대비 상품사이즈의 비율
        if($paper_name == "모조지 백색 80g"
            || $paper_name == "크라프트 갈색 57g"
            || $paper_name == "아트지 강접 백색 90g") {
            //옵셋
            $pan_price = 10000;
            if($amt <= 200) {
                $print_price = 80000;
            } else if($amt <= 500) {
                $print_price = 160000;
            } else if($amt <= 1000) {
                $print_price = 240000;
            } else if($amt <= 2000) {
                $print_price = 240000;
            } else if($amt <= 3000) {
                $print_price = 320000;
            } else if($amt <= 4000) {
                $print_price = 400000;
            } else if($amt <= 8000) {
                $print_price = 700000;
            } else if($amt <= 10000) {
                $print_price = 1000000;
            } else if($amt <= 15000) {
                $print_price = 1300000;
            } else if($amt <= 20000) {
                $print_price = 1800000;
            } else if($amt <= 40000) {
                $print_price = 3400000;
            } else if($amt <= 50000) {
                $print_price = 3800000;
            } else if($amt <= 60000) {
                $print_price = 4200000;
            } else if($amt <= 70000) {
                $print_price = 4600000;
            } else if($amt <= 80000) {
                $print_price = 5000000;
            } else if($amt <= 90000) {
                $print_price = 5400000;
            } else {
                $print_price = 7000000;
            }
            $print_price /= 2;
        } else if($paper_name == "투명데드롱 PET 투명 38g"
            || $paper_name == "은무데드롱 무광PET 은색 25g"
            || $paper_name == "유포지 무광PP 미색 80g") {
            //UV
            $pan_price = 16000;
            if($amt <= 200) {
                $print_price = 40000;
            } else if($amt <= 500) {
                $print_price = 160000;
            } else if($amt <= 1000) {
                $print_price = 160000;
            } else if($amt <= 2000) {
                $print_price = 240000;
            } else if($amt <= 3000) {
                $print_price = 320000;
            } else if($amt <= 4000) {
                $print_price = 400000;
            } else if($amt <= 8000) {
                $print_price = 700000;
            } else if($amt <= 10000) {
                $print_price = 1000000;
            } else if($amt <= 15000) {
                $print_price = 1300000;
            } else if($amt <= 20000) {
                $print_price = 1800000;
            } else if($amt <= 40000) {
                $print_price = 3400000;
            } else if($amt <= 50000) {
                $print_price = 3800000;
            } else if($amt <= 60000) {
                $print_price = 4200000;
            } else if($amt <= 70000) {
                $print_price = 4600000;
            } else if($amt <= 80000) {
                $print_price = 5000000;
            } else if($amt <= 90000) {
                $print_price = 5400000;
            } else {
                $print_price = 7000000;
            }
        }

        switch ($paper_name) {
            case "모조지 백색 80g" :
                $paper_price = 330;
                break;
            case "크라프트 갈색 57g" :
                $paper_price = 330;
                break;
            case "아트지 강접 백색 90g" :
                $paper_price = 330;
                break;
            case "투명데드롱 PET 투명 38g" :
                $paper_price = 450;
                break;
            case "은무데드롱 무광PET 은색 25g" :
                $paper_price = 450;
                break;
            case "유포지 무광PP 미색 80g" :
                $paper_price = 450;
                break;
        }

        switch ($param["st_bg"]) {
            case "유형1" :
                $tomson_knife_ratio = ($amt_ts * 1);
                break;
            case "유형2" :
                $tomson_knife_ratio = ($amt_ts * 3);
                break;
            case "유형3" :
                $tomson_knife_ratio = ($amt_ts * 6);
                break;
            case "유형4" :
            case "혼합유형" :
                $tomson_knife_ratio = ($amt_ts * 7);
                break;
        }

        $size = $size_width * $size_vert;
        if($size < 3000)
            $size = 3000;

        $tomson_count = $amt / 1000;
        if($tomson_count < 1) {
            $tomson_count = 1;
        }

        if($tomson_count > 15) {
            $tomson_count = 15;
        }

        $size = $size * (log10(3000) / log10($size));
        $ratio = $size / (773 * 520);

        $plate = ($pan_price * 4) * $ratio;
        $paper = $paper_price * $ratio * $amt;
        $print = $print_price * $ratio;
        $tomson_knife = 40000 * $ratio * $tomson_knife_ratio * $tomson_count; // 칼비
        $tomson = 50 * $ratio * $amt; // 도무송비

        //echo "plate : " . $plate . " paper : " . $paper . " tomson_knife : " . $tomson_knife . " tomson : " . $tomson;
        //$price = ($plate + $paper + $print + $tomson_knife + $tomson) * 3.4;

        if($param['product_margin'] == null || $param['product_margin'] == "") {
            $param['product_margin'] = 0;
        }
        $margin = ($param['product_margin'] + 100) / 100;
        $price = ($plate + $paper + $print + $tomson_knife + $tomson) * $margin;

        if($price < 8800) $price = 8800;
        return $this->util->ceilVal($price);

        $amt_form = [intval($param["amt_f1"]), intval($param["amt_f2"])
            ,intval($param["amt_f3"]), intval($param["amt_f4"])];

        $temp = [];
        $temp["amt"] = $amt;
        //$conn->debug = 1;
        $price_per = $dao->selectAfterTomsonPricePer($conn, $temp);

        $basic_price = doubleval($price_per["basic_price"]);
        $knife_per   = doubleval($price_per["knife_price_per"]);
        $paper_per   = doubleval($price_per["stick_paper_price_per"]);

        if (strpos($paper_name, "아트") === false
            && strpos($paper_name, "모조") === false
            && strpos($paper_name, "크라프트") === false) {
            $paper_per = doubleval($price_per["especial_paper_price_per"]);
        }

        if (strpos($paper_name, "크라프트") !== false
            || strpos($paper_name, "모조") !== false) {
            $paper_per += 0.02;
        }

        //echo "b_p : $basic_price / k_p : $knife_per / p_p : $paper_per\n";

        $area = (($size_width + 5.0) * ($size_vert + 5.0)) / 100.0;
        $area = round($area, 2);

        if ($area < 30.0) {
            $area = 30.0;
        }

        //echo "area : $area\n";

        $price_1 = $area * $paper_per * $amt;

        //echo "price_1 : $price_1\n";

        if ($price_1 < $basic_price) {
            $price_1 = $basic_price;
        }

        //echo "price_1 : $price_1\n";

        $col_name = null;
        $stan_name = explode('[', $stan_name)[0];

        switch ($stan_name) {
            case "유형1":
                $col_name = "typ1_price";
                break;
            case "유형2":
                $col_name = "typ2_price";
                break;
            case "유형3":
                $col_name = "typ3_price";
                break;
            case "유형4":
                $col_name = "typ4_price";
                break;
            case "혼합유형":
                $col_name = ["typ1_price", "typ2_price", "typ3_price", "typ4_price"];
                break;
        }

        //echo "col_name : $col_name\n";
        $price_2 = 0;
        $typ_price = 0;

        $chk_area = $size_width * $size_vert / 100.0;
        $chk_area = round($chk_area, 1);

        $temp["size_start"] = $chk_area;
        $temp["size_end"]   = $chk_area;

        //$conn->debug = 1;

        if ($stan_name == "혼합유형") {
            for ($i = 0; $i < 4; $i++) {
                $temp["col_name"] = $col_name[$i];
                $price_fields = $dao->selectAfterTomsonPrice($conn, $temp);

                $basic_price = doubleval($price_fields["basic_price"]);
                $typ_price  += doubleval($price_fields[$col_name[$i]]) * $amt_form[$i];
            }
        } else {
            $temp["col_name"] = $col_name;
            $price_fields = $dao->selectAfterTomsonPrice($conn, $temp);

            $basic_price = doubleval($price_fields["basic_price"]);
            $typ_price   = doubleval($price_fields[$col_name] * $amt_ts);
        }


        //echo "basic_price : $basic_price / typ_price : $typ_price\n";

        $price_2 += ($basic_price + $typ_price) * $knife_per * $amt / 1000;

        //echo "price2 : $price_2\n";

        $calc_price = round($price_1 + $price_2, -2) * 1.1;

        //echo "calc_price : $calc_price\n";

        return $this->util->ceilVal($calc_price);
    }
}
