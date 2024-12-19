<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * 프론트 가격확인 유틸
 *
 * @@ --> 낱장형에 있는 비교항목
 * !! --> 책자형에 있는 비교항목
 * ## --> NCR에 있는 비교항목
 * $$ --> 실사출력 현수막에 있는 비교항목
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/05/30 엄준현 생성
 *=============================================================================
 */
define("INC_PATH", $_SERVER["INC"]);

include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/NonStandardUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/OptPriceUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/AftPriceUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/BindingPriceUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/ActualPriceUtil.inc");
include_once(INC_PATH . "/com/nexmotion/job/front/product/ProductAoDAO.inc");
include_once(INC_PATH . "/common_lib/CalcPriceUtil.inc");
include_once(INC_PATH . "/common_lib/CommonUtil.inc");
include_once(INC_PATH . '/common_define/prdt_default_info.inc');
include_once(INC_PATH . "/define/front/product_info_class.inc");

class PriceValidationUtil {
    private $conn;
    private $dao;
    // 실사 제품 dao
    private $aoDAO;

    // 공통유틸
    private $util;
    // 계산형 유틸
    private $calcUtil;
    // 비규격 유틸
    private $nsUtil;
    // 옵션 유틸
    private $optUtil;
    // 일반 후공정 유틸
    private $aftUtil;
    // 일반 후공정중 제본 유틸
    private $aftBindingUtil;
    // 일반 후공정 중 자유형 도무송 유틸
    private $freeTomsonUtil;
    // 실사 후공정 유틸
    private $aoUtil;

    private $member_seqno;
    private $level;
    private $common_dvs;
    private $dvs_arr;
    private $fb;

    private $msg;
    private $msg_ptrn = "[FALSE %s] member_seqno : %s | detail : %s | category : %s | param : %s | search : %s\n";

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
            $this->util = new FrontCommonUtil();
        } else {
            $this->util = $param["util"];
        }

        $this->calcUtil = new CalcPriceUtil();
        $this->aftBindingUtil = new BindingPriceUtil();
        $this->aoUtil = new ActualPriceUtil();
        $this->aoDAO = new ProductAoDAO();

        $temp = [];
        $temp["conn"] = $this->conn;
        $temp["dao"]  = $this->dao;
        $temp["util"] = $this->util;
        $this->nsUtil  = new NonStandardUtil($temp);
        $this->aftUtil = new AftPriceUtil($temp);

        $temp["validate_flag"] = true;
        $this->optUtil = new OptPriceUtil($temp);

        $this->member_seqno = $param["member_seqno"];
        $this->level        = $param["level"];
        $this->common_dvs   = $param["common_dvs"];
        $this->dvs_arr      = $param["dvs_arr"];
        $this->fb           = $param["fb"];
    }

    /**
     * @brief 가격 확인
     *
     * @return 검증 성공시 true, 실패시 false
     */
    public function validatePrice(): bool {
        $common_dvs = $this->common_dvs;
        $dvs_arr    = $this->dvs_arr;
        $fb         = $this->fb;

        $ret = true;
        $expect_box = $fb["expect_box_num"];

        $temp = [];
        $booklet_param = [];
        if (!empty($common_dvs)) {
            $common_prefix = $fb["common_prdt_dvs"] . '_';

            $cate_sortcode = $fb[$common_prefix . "cate_sortcode"];
            $stan_dvs      = $fb[$common_prefix . "size_dvs"];
            $stan_mpcode   = $fb[$common_prefix . "size"];
            $cut_wid       = $fb[$common_prefix . "cut_wid_size"];
            $cut_vert      = $fb[$common_prefix . "cut_vert_size"];
            $work_wid      = $fb[$common_prefix . "work_wid_size"];
            $work_vert     = $fb[$common_prefix . "work_vert_size"];
            $amt           = $fb[$common_prefix . "amt"];
            $amt_unit      = $fb[$common_prefix . "amt_unit"];

            $booklet_param["cate_sortcode"] = $cate_sortcode;
            $booklet_param["stan_dvs"]      = $stan_dvs;
            $booklet_param["stan_mpcode"]   = $stan_mpcode;
            $booklet_param["cut_wid"]       = $cut_wid;
            $booklet_param["cur_vert"]      = $cut_vert;
            $booklet_param["work_wid"]      = $work_wid;
            $booklet_param["work_vert"]     = $work_vert;
            $booklet_param["amt"]           = $amt;

            $booklet_info_arr = [];
            $booklet_info_arr["paper_price"]  = 0;
            $booklet_info_arr["output_price"] = 0;
            $booklet_info_arr["print_price"]  = 0;
            $booklet_info_arr["aft_price"]    = 0;

            // !!등급할인율 비교
            $org_grade_sale_rate = doubleval($fb[$common_prefix . "grade_sale_rate"]);
            $temp["cate_sortcode"] = $cate_sortcode;
            $temp["grade"]         = $this->level;
            $grade_sale_rate = $this->selectGradeSaleRate($temp);

            if ($org_grade_sale_rate != $grade_sale_rate) {
                $line = __LINE__;
                $str = "등급할인율 불일치";
                $param_val  = $org_grade_sale_rate;
                $search_val = $grade_sale_rate;

                goto ERR;
            }
        }

        $dvs_arr_count = count($dvs_arr);
        for ($i = 0; $i < $dvs_arr_count; $i++) {
            $dvs = $dvs_arr[$i];
            $prefix = $dvs . '_';

            $cate_sortcode = $fb[$prefix . "cate_sortcode"];
            $sortcode_m = substr($cate_sortcode, 0, 6);

            $order_detail  = $fb[$prefix . "order_detail"];

            unset($temp);
            $temp["prefix"]     = $prefix;
            // 180427 추가 : 배송 덩어리
            $temp["expect_box"] = $expect_box;

            if ($sortcode_m === "007001") {
                // NCR
                $ret = $this->validateNcrPrice($temp);
            } else if ($cate_sortcode === "001001001") {
                // 카다로그, 브로셔
                $booklet_ret = $this->validateBookletPrice($temp, $booklet_param);
                $ret = $booklet_ret["success"];

                $booklet_info_arr["paper_price"]  += $booklet_ret["paper_price"];
                $booklet_info_arr["output_price"] += $booklet_ret["output_price"];
                $booklet_info_arr["print_price"]  += $booklet_ret["print_price"];
                $booklet_info_arr["aft_price"]    += $booklet_ret["aft_price"];
                $booklet_info_arr["page"]         += $booklet_ret["page"];
            } else if ($cate_sortcode === "002005003") {
                // 현수막
                $temp["order_detail"] = $order_detail;
                $ret = $this->validateAoPlacardPrice($temp);
            } else if ($cate_sortcode === "002003001") {
                // 배너거치대
                $ret = $this->validateAoRackPrice($temp);
            } else {
                // 낱장형
                $ret = $this->validateSheetPrice($temp);
            }

            if (!$ret) {
                return false;
            }
        }

        // !!책자형 총합가격 비교
        unset($temp);
        if (!empty($common_dvs)) {
            // 에러처리용 변수
            $line       = null;
            $str        = null;
            $param_val  = null; 
            $search_val = null;

            // 가격관련 정보
            $org_sell_price = intval($this->util->rmComma($fb[$common_prefix . "sell_price"]));
            $org_sale_price = intval($this->util->rmComma($fb[$common_prefix . "sale_price"]));

            $sum_paper_price  = $booklet_info_arr["paper_price"];
            $sum_output_price = $booklet_info_arr["output_price"];
            $sum_print_price  = $booklet_info_arr["print_price"];
            $sum_aft_price    = $booklet_info_arr["aft_price"];
            $sum_page         = $booklet_info_arr["page"];

            $sum_price = $sum_paper_price +
                         $sum_output_price +
                         $sum_print_price;

            // !!옵션 가격 비교
            $opt_param = [];
            $opt_param["prefix"] = $common_prefix;
            $opt_param["sell_price"] = $sum_price;

            // 옵션 가격 비교를 위한 덩어리 데이터
            $opt_param["expect_box"] = $expect_box;

            $opt_ret = $this->validateOptPrice($opt_param);
            if (!$opt_ret["success"]) {
                $line = __LINE__;
                $str = $opt_ret["str"];
                $param_val  = $opt_ret["param"];
                $search_val = $opt_ret["search"];

                goto ERR;
            }
            $opt_price = $opt_ret["search"];
            unset($opt_param);

            // 종이, 출력, 인쇄 가격으로 등급할인금액 계산
            $grade_sale = $this->util->calcPrice($grade_sale_rate, $sum_price);
            $grade_sale = $this->util->ceilVal($grade_sale);

            // 옵션/후공정 가격은 할인적용을 받지 않으므로 
            // 그 후에 옵션, 후공정 가격 가산
            $sum_price += $sum_aft_price + $opt_price;

            // 제본관련 정보
            $aft_binding_mpcode = $fb[$common_prefix . "binding_val"];
            $aft_binding_price  = $fb[$common_prefix . "binding_price"];

            $temp["prefix"]         = $common_prefix;
            $temp["page"]           = $sum_page;
            $temp["binding_depth1"] = $fb[$common_prefix . "binding_depth1"];
            $temp["size_name"]      = $fb[$common_prefix . "size_name"];

            $binding_ret = $this->validateAftPrice($temp);
            // !!제본 가격 비교
            if (!$binding_ret["success"]) {
                $line = __LINE__;
                $str = $binding_ret["str"];
                $param_val  = $binding_ret["param"];
                $search_val = $binding_ret["search"];

                goto ERR;
            }

            $sum_price += $binding_ret["search"];

            $grade_sale += $sum_price;
            $grade_sale  = $this->util->ceilVal($grade_sale);

            // !!판매 가격 비교
            if (intval($sum_price) !== intval($org_sell_price)) {
                $line = __LINE__;
                $str = "총 판매가격 불일치(할인미적용)";
                $param_val  = $sum_price;
                $search_val = $org_sell_price;
            }

            // !!비교 : 할인가격 비교
            if (intval($grade_sale) !== intval($org_sale_price)) {
                $line = __LINE__;
                $str = "총 판매가격 불일치(할인적용)";
                $param_val  = $org_sale_price;
                $search_val = $grade_sale;

                goto ERR;
            }
        }

        return $ret;

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return false;
    }

    /**
     * @brief 낱장형 가격 확인
     *
     * @param $param = 일부 공통데이터
     */
    private function validateSheetPrice($param): bool {
        // 에러처리용 변수
        $line       = null;
        $str        = null;
        $param_val  = null; 
        $search_val = null;

        $conn     = $this->conn;
        $dao      = $this->dao;
        $util     = $this->util;
        $calcUtil = $this->calcUtil;
        $nsUtil   = $this->nsUtil;

        $prefix = $param["prefix"];
        $fb     = $this->fb;

        $cate_sortcode = $fb[$prefix . "cate_sortcode"];
        $sortcode_t    = substr($cate_sortcode, 0, 3);
        $amt           = doubleval($fb[$prefix . "amt"]);
        $count         = $fb[$prefix . "count"];
        $count         = empty($count) ? 1 : intval($count);
        $expect_box    = $param["expect_box"];

        $pos_arr       = PrdtDefaultInfo::POSITION_NUMBER[$cate_sortcode];

        // 카테고리 기본선택값, 명함 등에서 자리수, 규격명 변조확인용
        $default_sel_arr = ProductDefaultSel::DEFAULT_SEL[$cate_sortcode];

        // 카테고리 정보
        $cate_info_arr = $dao->selectCateInfo($conn, $cate_sortcode);
        $mono_dvs   = $cate_info_arr["mono_dvs"];
        $amt_unit   = $cate_info_arr["amt_unit"];
        $tmpt_dvs   = $cate_info_arr["tmpt_dvs"];
        $flattyp_yn = $cate_info_arr["flattyp_yn"];

        // 규격/비규격 구분
        $stan_dvs  = $fb[$prefix . "size_dvs"];
        // 자리수
        $pos_num   = $fb[$prefix . "manu_pos_num"];
        // 규격명
        $size_name = $fb[$prefix . "size_name"];

        // 종이코드
        $paper_mpcode = $fb[$prefix . "paper"];
        // 사이즈 맵핑코드
        $stan_mpcode  = $fb[$prefix . "size"];
        // 인쇄 맵핑코드
        $print_bef_mpcode     = intval($fb[$prefix . "bef_tmpt"]);
        $print_aft_mpcode     = intval($fb[$prefix . "aft_tmpt"]);
        $print_bef_add_mpcode = intval($fb[$prefix . "bef_add_tmpt"]);
        $print_aft_add_mpcode = intval($fb[$prefix . "aft_add_tmpt"]);

        // 파라미터 넘어온 가격(검색결과와 비교할 가격)
        $org_prdt_price = $util->rmComma($fb[$prefix . "prdt_price"]);
        $org_sell_price = $util->rmComma($fb[$prefix . "sell_price"]);
        $org_sale_price = $util->rmComma($fb[$prefix . "sale_price"]);
        $org_grade_sale_rate = doubleval($fb[$prefix . "grade_sale_rate"]);

        $mono_dvs = ($mono_dvs === '1' || $mono_dvs === '2') ? 0 : 1;

        $temp = [];

        // 등급할인율
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["grade"]         = $this->level;
        $grade_sale_rate = $this->selectGradeSaleRate($temp);

        // @@비교 : 파라미터로 넘어온 등급할인율과 검색결과랑 비교
        if (intval($org_grade_sale_rate) != intval($grade_sale_rate)) {
            $line = __LINE__;
            $str = "등급할인율 불일치";
            $param_val  = $org_grade_sale_rate;
            $search_val = $grade_sale_rate;

            goto ERR;
        }

        // 사이즈 정보 검색
        $size_info = $dao->selectSizeNameAffil($conn, ["mpcode" => $stan_mpcode]);

        // 비규격일 때 처리
        unset($temp);
        if ($stan_dvs === "manu") {
            if ($sortcode_t === "003") {
                // 명함카테고리는 기본 규격으로 자리수를 비교
                $default_size = $default_sel_arr["size"];

                $temp["cate_sortcode"] = $cate_sortcode;
                $temp["stan_name"]     = $default_size;

                $cut_work_info = $dao->selectCutWorkStanInfo($conn, $temp);

                unset($temp);
                $temp["default_wid"]  = $cut_work_info["cut_wid_size"];
                $temp["default_vert"] = $cut_work_info["cut_vert_size"];
                $temp["cut_wid"]  = $fb[$prefix . "cut_wid_size"];
                $temp["cut_vert"] = $fb[$prefix . "cut_vert_size"];

                $org_pos_num = $nsUtil->ncCalcPos($temp);

                // @@비교 : 파라미터로 넘어온 자리수와 계산된 자리수 비교
                if ($org_pos_num != $pos_num) {
                    $line = __LINE__;
                    $str = "자리수 불일치";
                    $param_val  = $pos_num .
                                  '(' . $fb[$prefix . "cut_wid_size"] . '*' .
                                  $fb[$prefix . "cut_vert_size"] . ')';
                    $search_val = $org_pos_num .
                                  '(' . $cut_work_info["cut_wid_size"] . '*' .
                                  $cut_work_info["cut_vert_size"] . ')';

                    goto ERR;
                }

            } else if ($sortcode_t === "001" || $sortcode_t === "005") {
                // 비규격일 때 상업인쇄류 카테고리는 유사 사이즈 검색
                $temp["stan_mpcode"]   = $stan_mpcode;
                $temp["cate_sortcode"] = $cate_sortcode;
                $temp["cut_wid"]  = $fb[$prefix . "cut_wid_size"];
                $temp["cut_vert"] = $fb[$prefix . "cut_vert_size"];

                // @@비교 : 파라미터로 넘어온 stan_mpcode와 검색한 값 비교
                if ($this->validateSimilarSize($temp)) {
                    $line = __LINE__;
                    $str = "cate_stan_mpcode 불일치";
                    $param_val  = $stan_mpcode;
                    $search_val = $similar_info_arr["mpcode"];

                    goto ERR;
                }
            }
        } else {
            // @@비교 : 규격일 때 파라미터로 넘어온 규격명과 검색결과랑 비교
            if ($stan_dvs === "stan" && $size_name !== $size_info["name"]) {
                $line = __LINE__;
                $str = "규격명 불일치";
                $param_val  = $size_name;
                $search_val = $size_info["name"];

                goto ERR;
            }
        }

        
        // 가격 테이블명 검색
        $price_tb  = "ply_price";
        // 후공정 가격과 옵션가격을 더하기 위한 기준가격
        // 확정형 - sale_price / 계산형 - sum_price
        $base_price = 0;

        unset($temp);
        if ($mono_dvs === 0) {
            // 확정형
            //-------------------------------------------------------
            // 등급할인율과 기본 판매가격만 비교함
            // 할인금액은 후공정과 옵션 때문에 밑에서 비교
            //-------------------------------------------------------
            // 기준가격(정상판매가) 검색, 부가세 계산
            $temp["table_name"]           = $price_tb;
            $temp["member_seqno"]         = $this->member_seqno;
            $temp["cate_sortcode"]        = $cate_sortcode;
            $temp["paper_mpcode"]         = $paper_mpcode;
            $temp["bef_print_mpcode"]     = $print_bef_mpcode;
            $temp["bef_add_print_mpcode"] = $print_bef_add_mpcode;
            $temp["aft_print_mpcode"]     = $print_aft_mpcode;
            $temp["aft_add_print_mpcode"] = $print_aft_add_mpcode;
            $temp["stan_mpcode"]          = $stan_mpcode;
            $temp["amt"]                  = $amt;

            // 판매가격
            $price_rs = $dao->selectPrdtPlyPrice($conn, $temp);
            // 수량회원할인정보
            $amt_sale_rs = $dao->selectAmtMemberCateSale($conn, $temp);

            $sell_price = doubleval($price_rs["new_price"]);
            $sell_price = $util->ceilVal($sell_price);

            if ($stan_dvs === "manu" && $sortcode_t === "004") {
                // 스티커 비규격일 경우 판매가격 계산
                unset($temp);
                $temp["cate_sortcode"] = $cate_sortcode;
                $temp["st_bg"] = $fb["st_bg"];
                $temp["wid"]   = $fb[$prefix . "cut_wid_size"];
                $temp["vert"]  = $fb[$prefix . "cut_vert_size"];
                $temp["amt"]   = $amt;
                $temp["price"] = $sell_price / 1.1;
                $temp["grade_sale"] = $grade_sale_rate;
                $temp["paper_info"] = $fb[$prefix . "paper_name"];

                $sell_price = $nsUtil->stCalcPrice($temp);
                $sell_price = $util->ceilVal($sell_price);
            } else if ($cate_sortcode === "004003009") {
                // 자유형 도무송 스티커 일 때 판매가격 계산
                unset($temp);
                $temp["paper_name"] = $fb[$prefix . "paper_name"];
                $temp["stan_name"]  = $fb[$prefix . "size_name"];
                $temp["amt"]        = $amt;
                $temp["amt_ts"]     = $fb["st_form_amt"];
                $temp["amt_f1"]     = $fb["form_1"];
                $temp["amt_f2"]     = $fb["form_2"];
                $temp["amt_f3"]     = $fb["form_3"];
                $temp["amt_f4"]     = $fb["form_4"];
                $temp["size_width"] = $fb[$prefix . "cut_wid_size"];
                $temp["size_vert"]  = $fb[$prefix . "cut_vert_size"];
                $sell_price = $this->aftUtil->getFreeTomsonPrice($temp);
            } else {
                $grade_sale = $util->calcPrice($grade_sale_rate, $sell_price);
                $sale_price = $sell_price + $grade_sale;
            }

            $amt_sale_rate = $amt_sale_rs["rate"];
            $amt_sale_aplc = intval($amt_sale_rs["aplc_price"]);

            // @@비교 : 파라미터로 넘어온 상품가격과 검색결과랑 비교
            if (intval($org_prdt_price) != intval($sell_price)) {
                $line = __LINE__;
                $str = "판매가격 불일치";
                $param_val  = $org_prdt_price;
                $search_val = $sell_price;

                goto ERR;
            }

            $base_price = $sell_price;
        } else {
            // 계산형
            $temp["conn"] = $this->conn;
            $temp["dao"]  = $this->dao;

            $temp["cate_sortcode"] = $cate_sortcode;
            $temp["amt_unit"]      = $amt_unit;
            $temp["flattyp_yn"]    = $flattyp_yn;

            $temp["amt"]     = $amt;
            $temp["page"]    = 2;
            $temp["pos_num"] = $pos_arr[$size_info["name"]];
            $temp["affil"]   = $size_info["affil"];

            $temp["cate_paper_mpcode"]    = $paper_mpcode;
            $temp["cate_output_mpcode"]   = $stan_mpcode;
            $temp["bef_print_mpcode"]     = $print_bef_mpcode;
            $temp["bef_add_print_mpcode"] = $print_bef_add_mpcode;
            $temp["aft_print_mpcode"]     = $print_aft_mpcode;
            $temp["aft_add_print_mpcode"] = $print_aft_add_mpcode;

            // 마스터 인쇄일 때, 수량 낱장여부 별도처리
            if ($cate_sortcode === "007002001") {
                $temp["flattyp_yn"] = 'N';
                $temp["amt"] =
                    PrdtDefaultInfo::MST_GROUP * 2 * $amt;
            }

            $calcUtil->setData($temp);
            unset($temp);

            $temp["bef_print_name"] = $fb[$prefix . "bef_tmpt_name"];
            $temp["aft_print_name"] = $fb[$prefix . "aft_tmpt_name"];
            $temp["bef_add_print_name"] = $fb[$prefix . "bef_add_tmpt_name"];
            $temp["aft_add_print_name"] = $fb[$prefix . "aft_add_tmpt_name"];

            $paper_price  = $util->ceilVal($calcUtil->calcPaperPrice($temp));
            $print_price  = $util->ceilVal($calcUtil->calcSheetPrintPrice());
            $output_price = $util->ceilVal($calcUtil->calcSheetOutputPrice());
            $sum_price    = $paper_price + $print_price + $output_price;

            // @@비교 : 파라미터로 넘어온 판매가격과 검색결과랑 비교
            if (intval($org_prdt_price) != intval($sum_price)) {
                $line = __LINE__;
                $str = "종이/출력/인쇄 합산가격 불일치";
                $param_val  = $org_prdt_price;
                $search_val = $sum_price;

                goto ERR;
            }

            $base_price = $sum_price;
            $amt_sale_price = 0;
        }

        // @@옵션 가격 비교
        $opt_param = [];
        $opt_param["prefix"]       = $prefix;
        $opt_param["sell_price"]   = $base_price;
        $opt_param["paper_mpcode"] = $paper_mpcode;
        $opt_param["size_affil"]   = $size_info["affil"];
        $opt_param["amt"]          = $amt;
        // 180427 추가 : 배송 덩어리 데이터
        $opt_param["expect_box"]   = $expect_box;

        $opt_ret = $this->validateOptPrice($opt_param);
        if (!$opt_ret["success"]) {
            $line = __LINE__;
            $str = $opt_ret["str"];
            $param_val  = $opt_ret["param"];
            $search_val = $opt_ret["search"];

            goto ERR;
        }
        $opt_price = $opt_ret["search"];
        unset($opt_param);
        unset($opt_ret);

        // @@후공정 가격 비교
        $aft_param = [];
        $aft_param["prefix"] = $prefix;

        $aft_ret = $this->validateAftPrice($aft_param);

        if (!$aft_ret["success"]) {
            $line = __LINE__;
            $str = $aft_ret["str"];
            $param_val  = $aft_ret["param"];
            $search_val = $aft_ret["search"];

            goto ERR;
        }
        $org_aft_price = $aft_ret["param"];
        $aft_price     = $aft_ret["search"];

        // 판매가격
        $pos_num = empty($pos_num) ? 1 : intval($pos_num);
        //$sum_price = ($base_price + $opt_price + $aft_price) * $count * $pos_num;
        // 180427 추가 : 옵션가격은 count에 이미 영향을 받음, 따로 추가해야함.
        $sum_price  = ($base_price + $aft_price) * $count * $pos_num;
        $sum_price += $opt_price;

        // @@비교 : 파라미터로 넘어온 판매가격와 계산된 판매가격 비교
        if (intval($sum_price) !== intval($org_sell_price)) {
            $line = __LINE__;
            $str = "총 판매가격 불일치(할인미적용)";
            $param_val  = $org_sell_price;
            //$param_val  = $base_price . "&nbsp" . $opt_price . "&nbsp" . $aft_price . "&nbsp" . $count . "&nbsp" . $pos_num;
            $search_val = $sum_price;

            exit;

            goto ERR;
        }

        $amt_sale_rate = empty($amt_sale_rate) ? 0 : intval($amt_sale_rate);
        $amt_sale_aplc = empty($amt_sale_aplc) ? 0 : intval($amt_sale_aplc);

        $base_price  = $base_price * $count * $pos_num;
        // 등급할인
        $grade_sale  = $util->calcPrice($grade_sale_rate, $base_price);
        $grade_sale  = $util->ceilVal($grade_sale);
        // 수량할인
        $tmp_price = $base_price + $grade_sale;
        $amt_sale = $util->calcPrice($amt_sale_rate, $tmp_price)
                    + $amt_sale_aplc;
        $amt_sale = $util->ceilVal($amt_sale);

        $grade_sale += $sum_price;
        $grade_sale += $amt_sale_price;

        // @@비교 : 파라미터로 넘어온 상품가격과 재계산한 상품가격 비교
        if (intval($grade_sale) !== intval($org_sale_price)) {
            $line = __LINE__;
            $str = "총 판매가격 불일치(할인적용)";
            $param_val  = $grade_sale;
            $search_val = $org_sale_price;

            goto ERR;
        }

        return true;

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return false;
    }

    /**
     * @brief 책자형 가격 확인
     *
     * @param $param = 구분별 공통데이터
     * @param $param = 카테고리 전체 공통데이터
     */
    private function validateBookletPrice($param, $booklet_param): array {
        $line       = null;
        $str        = null;
        $param_val  = null;
        $search_val = null;

        $conn     = $this->conn;
        $dao      = $this->dao;
        $util     = $this->util;
        $calcUtil = $this->calcUtil;
        $nsUtil   = $this->nsUtil;

        $prefix = $param["prefix"];
        $fb     = $this->fb;

        $cate_sortcode = $booklet_param["cate_sortcode"];
        $amt           = intval($booklet_param["amt"]);
        $count         = 1;
        $page          = $fb[$prefix . "page"];
        $stan_dvs      = $booklet_param["stan_dvs"];
        $stan_mpcode   = $booklet_param["stan_mpcode"];
        $pos_arr       = PrdtDefaultInfo::POSITION_NUMBER[$cate_sortcode];

        // 카테고리 정보
        $cate_info_arr = $dao->selectCateInfo($conn, $cate_sortcode);
        $mono_dvs   = $cate_info_arr["mono_dvs"];
        $amt_unit   = $cate_info_arr["amt_unit"];
        $tmpt_dvs   = $cate_info_arr["tmpt_dvs"];
        $flattyp_yn = $cate_info_arr["flattyp_yn"];

        $size_name = $fb[$prefix . "size_name"];

        $paper_mpcode = $fb[$prefix . "paper"];

        $print_bef_mpcode = $fb[$prefix . "bef_tmpt"];
        $print_aft_mpcode = $fb[$prefix . "aft_tmpt"];
        $print_bef_add_mpcode = $fb[$prefix . "bef_add_tmpt"];
        $print_aft_add_mpcode = $fb[$prefix . "aft_add_tmpt"];

        $print_bef_name = $fb[$prefix . "bef_tmpt_name"];
        $print_aft_name = $fb[$prefix . "aft_tmpt_name"];
        $print_bef_add_name = $fb[$prefix . "bef_add_tmpt_name"];
        $print_aft_add_name = $fb[$prefix . "aft_add_tmpt_name"];

        // 비규격일 때 상업인쇄류 카테고리는 유사 사이즈 검색
        $temp = [];
        if ($stan_dvs === "manu") {
            $temp["stan_mpcode"]   = $stan_mpcode;
            $temp["cate_sortcode"] = $cate_sortcode;
            $temp["cut_wid"]  = $booklet_param["cut_wid"];
            $temp["cut_vert"] = $booklet_param["cut_vert"];

            // !!비교 : 파라미터로 넘어온 stan_mpcode와 검색한 값 비교
            if ($this->validateSimilarSize($temp)) {
                $line = __LINE__;
                $str = "cate_stan_mpcode 불일치";
                $param_val  = $stan_mpcode;
                $search_val = $similar_info_arr["mpcode"];

                goto ERR;
            }
        } else {
            // !!비교 : 규격일 때 파라미터로 넘어온 규격명과 검색결과랑 비교
            $size_info = $dao->selectSizeNameAffil($conn, ["mpcode" => $stan_mpcode]);
            if ($stan_dvs === "stan" && $size_name !== $size_info["name"]) {
                $line = __LINE__;
                $str = "규격명 불일치";
                $param_val  = $size_name;
                $search_val = $size_info["name"];

                goto ERR;
            }
        }

        unset($temp);
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["amt_unit"]      = $amt_unit;
        $temp["flattyp_yn"]    = $flattyp_yn;

        $temp["amt"]     = $amt;
        $temp["page"]    = $page;
        $temp["pos_num"] = $pos_arr[$size_info["name"]];
        $temp["affil"]   = $size_info["affil"];

        $temp["cate_paper_mpcode"]  = $paper_mpcode;
        $temp["cate_output_mpcode"] = $stan_mpcode;

        $temp["bef_print_mpcode"]     = $print_bef_mpcode;
        $temp["aft_print_mpcode"]     = $print_aft_mpcode;
        $temp["bef_add_print_mpcode"] = $print_bef_add_mpcode;
        $temp["aft_add_print_mpcode"] = $print_aft_add_mpcode;

        $calcUtil->setData($temp);

        $print_name_arr = [];
        $print_name_arr["bef_print_name"] = $print_bef_name;
        $print_name_arr["aft_print_name"] = $print_aft_name;
        $print_name_arr["bef_add_print_name"] = $print_bef_add_name;
        $print_name_arr["aft_add_print_name"] = $print_aft_add_name;

        $paper_price  =
            intval($util->ceilVal($calcUtil->calcPaperPrice($print_name_arr)));
        $print_price  =
            intval($util->ceilVal($calcUtil->calcBookletPrintPrice()));
        $output_price =
            intval($util->ceilVal($calcUtil->calcBookletOutputPrice()));
        $sum_price   = $paper_price + $print_price + $output_price;

        $org_paper_price  = intval($fb[$prefix . "paper_price"]);
        $org_print_price  = intval($fb[$prefix . "print_price"]);
        $org_output_price = intval($fb[$prefix . "output_price"]);

        // !!비교 : 파라미터로 넘어온 종이/출력/인쇄 가격과 계산된 가격 비교
        if ($paper_price !== $org_paper_price) {
            $line = __LINE__;
            $str = "종이가격 불일치";
            $param_val  = $org_paper_price;
            $search_val = $paper_price;

            goto ERR;
        }
        if ($output_price !== $org_output_price) {
            $line = __LINE__;
            $str = "출력가격 불일치";
            $param_val  = $org_output_price;
            $search_val = $output_price;

            goto ERR;
        }
        if ($print_price !== $org_print_price) {
            $line = __LINE__;
            $str = "인쇄가격 불일치";
            $param_val  = $org_print_price;
            $search_val = $print_price;

            goto ERR;
        }

        // !!후공정 가격 비교
        $aft_param = [];
        $aft_param["prefix"]    = $prefix;
        $aft_param["amt_unit"]  = $amt_unit;
        $aft_param["page"]      = $page;
        $aft_param["size_name"] = $size_name;

        $aft_ret = $this->validateAftPrice($aft_param);

        if (!$aft_ret["success"]) {
            $line = __LINE__;
            $str = $aft_ret["str"];
            $param_val  = $aft_ret["param"];
            $search_val = $aft_ret["search"];

            goto ERR;
        }

        return [
            "success" => true,
            "page"    => intval($page),
            "paper_price"  => $paper_price,
            "output_price" => $output_price,
            "print_price"  => $print_price,
            "aft_price"    => intval($aft_ret["search"])
        ];

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return [
                "success" => false,
                "page"    => -10,
                "paper_price"  => -100,
                "output_price" => -100,
                "print_price"  => -100,
                "aft_price"    => -100
            ];
    }

    /**
     * @brief NCR 가격 확인
     *
     * @param $param = 구분별 공통데이터
     */
    private function validateNcrPrice($param): bool {
        // 에러처리용 변수
        $line       = null;
        $str        = null;
        $param_val  = null; 
        $search_val = null;

        $conn     = $this->conn;
        $dao      = $this->dao;
        $util     = $this->util;
        $calcUtil = $this->calcUtil;
        $nsUtil   = $this->nsUtil;

        $prefix = $param["prefix"];
        $fb     = $this->fb;

        $cate_sortcode = $fb[$prefix . "cate_sortcode"];
        $pos_arr = PrdtDefaultInfo::POSITION_NUMBER[$cate_sortcode];
        $diff_yn = empty($fb[$prefix . "diff_yn"]) ? false : true;
        $amt     = PrdtDefaultInfo::MST_GROUP * intval($fb[$prefix . "amt"]);
        // 규격/비규격 구분
        $stan_dvs  = $fb[$prefix . "size_dvs"];
        // 규격명
        $size_name = $fb[$prefix . "size_name"];
        // 규격의 자리수
        $pos_num = $fb[$prefix . "pos_num"];
        // 페이지수
        $page    = 2;
        // 도수명
        $print_bef_name = $fb[$prefix . "bef_tmpt_name"];
        $print_aft_name = '';
        $print_bef_add_name = '';
        $print_aft_add_name = '';

        // ##비교 : 자리수 비교
        if (intval($pos_num) !== intval($pos_arr[$size_name])) {
            $line = __LINE__;
            $str = "자리수 불일치";
            $param_val  = $pos_num;
            $search_val = $pos_arr[$size_name];

            goto ERR;
        }

        // ##비교 : 페이지수 비교
        if ($page !== intval($fb[$prefix . "page"])) {
            $line = __LINE__;
            $str = "페이지수 불일치";
            $param_val  = $fb[$prefix . "page"];
            $search_val = $page;

            goto ERR;
        }

        // 파라미터 넘어온 가격(검색결과와 비교할 가격)
        $org_prdt_price = $util->rmComma($fb[$prefix . "prdt_price"]);
        $org_sell_price = $util->rmComma($fb[$prefix . "sell_price"]);
        $org_sale_price = $util->rmComma($fb[$prefix . "sale_price"]);
        $org_grade_sale_rate = doubleval($fb[$prefix . "grade_sale_rate"]);

        // 등급할인율
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["grade"]         = $this->level;
        $grade_sale_rate = $this->selectGradeSaleRate($temp);

        // ##비교 : 파라미터로 넘어온 등급할인율과 검색결과랑 비교
        if (intval($org_grade_sale_rate) != intval($grade_sale_rate)) {
            $line = __LINE__;
            $str = "등급할인율 불일치";
            $param_val  = $org_grade_sale_rate;
            $search_val = $grade_sale_rate;

            goto ERR;
        }

        // 종이코드
        $paper_mpcode_arr = explode('|', $fb[$prefix . "paper_list"]);
        // 사이즈 맵핑코드
        $stan_mpcode  = $fb[$prefix . "size"];
        // 인쇄 맵핑코드
        $print_bef_mpcode     = intval($fb[$prefix . "bef_tmpt"]);
        $print_aft_mpcode     = intval($fb[$prefix . "aft_tmpt"]);
        $print_bef_add_mpcode = intval($fb[$prefix . "bef_add_tmpt"]);
        $print_aft_add_mpcode = intval($fb[$prefix . "aft_add_tmpt"]);

        // 카테고리 정보
        $cate_info_arr = $dao->selectCateInfo($conn, $cate_sortcode);
        $mono_dvs   = $cate_info_arr["mono_dvs"];
        $amt_unit   = $cate_info_arr["amt_unit"];
        $tmpt_dvs   = $cate_info_arr["tmpt_dvs"];
        $flattyp_yn = $cate_info_arr["flattyp_yn"];
        // 사이즈 정보 검색
        $size_info  = $dao->selectSizeNameAffil($conn, ["mpcode" => $stan_mpcode]);
        $size_affil = $size_info["affil"];

        $temp = [];
        if ($stan_dvs === "manu") {
            // 비규격일 때 유사 사이즈 검색
            $temp["stan_mpcode"]   = $stan_mpcode;
            $temp["cate_sortcode"] = $cate_sortcode;
            $temp["cut_wid"]  = $fb[$prefix . "cut_wid_size"];
            $temp["cut_vert"] = $fb[$prefix . "cut_vert_size"];

            // ##비교 : 파라미터로 넘어온 stan_mpcode와 검색한 값 비교
            if ($this->validateSimilarSize($temp)) {
                $line = __LINE__;
                $str = "cate_stan_mpcode 불일치";
                $param_val  = $stan_mpcode;
                $search_val = $similar_info_arr["mpcode"];

                goto ERR;
            }
        } else {
            // ##비교 : 규격일 때 파라미터로 넘어온 규격명과 검색결과랑 비교
            if ($stan_dvs === "stan" && $size_name !== $size_info["name"]) {
                $line = __LINE__;
                $str = "규격명 불일치";
                $param_val  = $size_name;
                $search_val = $size_info["name"];

                goto ERR;
            }
        }

        unset($temp);
        $temp["conn"] = $this->conn;
        $temp["dao"]  = $this->dao;

        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["amt_unit"]      = $amt_unit;
        $temp["flattyp_yn"]    = 'N';

        $temp["amt"]     = $amt;
        $temp["page"]    = $page;
        $temp["pos_num"] = $pos_num;
        $temp["affil"]   = $size_affil;

        $temp["cate_output_mpcode"] = $stan_mpcode;

        $temp["bef_print_mpcode"]     = $print_bef_mpcode;
        $temp["aft_print_mpcode"]     = $print_aft_mpcode;
        $temp["bef_add_print_mpcode"] = $print_bef_add_mpcode;
        $temp["aft_add_print_mpcode"] = $print_aft_add_mpcode;

        $calcUtil->setData($temp);

        $sum_paper_price  = 0;
        $sum_print_price  = 0;
        $sum_output_price = 0;

        $paper_mpcode_arr_count = count($paper_mpcode_arr);
        $is_fst = true;
        for ($i = 0; $i < $paper_mpcode_arr_count ; $i++) {
            $paper_mpcode = $paper_mpcode_arr[$i];

            $calcUtil->setCatePaperMpcode($paper_mpcode);

            $paper_price = $util->ceilVal($calcUtil->calcPaperPrice($temp));

            $sum_paper_price  += $paper_price;

            if ($diff_yn) {
                // 내용 다름
                $print_price  = $util->ceilVal($calcUtil->calcSheetPrintPrice());
                $output_price = $util->ceilVal($calcUtil->calcSheetOutputPrice());

                $sum_print_price  += $print_price;
                $sum_output_price += $output_price;
            } else {
                // 내용 같음
                if ($is_fst) {
                    $calcUtil->setAmt($amt * $paper_mpcode_arr_count);
                    $calcUtil->calcRealPaperAmt();

                    $print_price  = $util->ceilVal($calcUtil->calcSheetPrintPrice());
                    $output_price = $util->ceilVal($calcUtil->calcSheetOutputPrice());

                    $sum_print_price  += $print_price;
                    $sum_output_price += $output_price;
                    $is_fst = false;

                    $calcUtil->setAmt($amt);
                }
            }
        }

        $sum_price = $sum_paper_price + $sum_print_price + $sum_output_price;

        // ##비교 : 파라미터로 넘어온 판매가격과 검색결과랑 비교
        if (intval($org_prdt_price) != intval($sum_price)) {
            $line = __LINE__;
            $str = "종이/출력/인쇄 합산가격 불일치";
            $param_val  = $org_prdt_price;
            $search_val = $sum_price;

            goto ERR;
        }

        $grade_sale  = $util->calcPrice($grade_sale_rate, $sum_price);
        $grade_sale  = $util->ceilVal($grade_sale);
        $grade_sale += $sum_price;

        // ##옵션 가격 비교
        /*
        $opt_param = [];
        $opt_param["prefix"] = $prefix;
        $opt_param["sell_price"] = $sum_price;
        $opt_param["paper_mpcode"] = $paper_mpcode;
        $opt_param["size_affil"]   = $size_info["affil"];

        $opt_ret = $this->validateOptPrice($opt_param);
        if (!$opt_ret["success"]) {
            $line = __LINE__;
            $str = $opt_ret["str"];
            $param_val  = $opt_ret["param"];
            $search_val = $opt_ret["search"];

            goto ERR;
        }
        $opt_price = $opt_ret["search"];
        unset($opt_param);
        */

        // ##후공정 가격 비교
        $aft_param = [];
        $aft_param["prefix"] = $prefix;
        $aft_param["page"]           = $page;
        $aft_param["binding_depth1"] = "떡제본";
        $aft_param["size_name"]      = $size_name;

        $aft_ret = $this->validateAftPrice($aft_param);

        if (!$aft_ret["success"]) {
            $line = __LINE__;
            $str = $aft_ret["str"];
            $param_val  = $aft_ret["param"];
            $search_val = $aft_ret["search"];

            goto ERR;
        }
        $grade_sale += $aft_ret["search"];

        // !!비교 : 할인가격 비교
        if (intval($grade_sale) !== intval($org_sale_price)) {
            $line = __LINE__;
            $str = "총 판매가격 불일치(할인적용)";
            $param_val  = $grade_sale;
            $search_val = $org_sale_price;

            goto ERR;
        }

        return true;

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return false;
    }

    /**
     * @brief 실사제품 현수막 가격 확인
     *
     * @param $param = 구분별 공통데이터
     */
    private function validateAoPlacardPrice($param): bool {
        // 에러처리용 변수

        $line       = null;
        $str        = null;
        $param_val  = null; 
        $search_val = null;

        $conn     = $this->conn;
        $dao      = $this->aoDAO;
        $util     = $this->util;
        $aoUtil   = $this->aoUtil;

        $prefix = $param["prefix"];
        $fb     = $this->fb;

        $cate_sortcode = $fb[$prefix . "cate_sortcode"];
        $size_type     = $fb["size_type"];
        $amt           = intval($fb[$prefix . "amt"]);
        
        // 파라미터 넘어온 가격(검색결과와 비교할 가격)
        $org_prdt_price = $util->rmComma($fb[$prefix . "prdt_price"]);
        $org_sell_price = $util->rmComma($fb[$prefix . "sell_price"]);
        $org_sale_price = $util->rmComma($fb[$prefix . "sale_price"]);
        $org_grade_sale_rate = doubleval($fb[$prefix . "grade_sale_rate"]);
        // 현수막 사이즈
        $wid  = 0;
        $vert = 0;
        $adh_price = 0;

        switch ($size_type) {
            case "가로분할" :
                $wid  = $fb[$prefix . "cut_wid_size"];
                $vert = $fb[$prefix . "sel_cut_vert_size"];
                break;
            case "세로분할" :
                $wid  = $fb[$prefix . "sel_cut_wid_size"];
                $vert = $fb[$prefix . "cut_vert_size"];
                break;
            case "직접입력" : 
                $wid  = $fb[$prefix . "cut_wid_size"];
                $vert = $fb[$prefix . "cut_vert_size"];

                $order_detail    = $param["order_detail"];
                $divide_info     = explode('/', $order_detail)[4];  
    
                $divide_info_met = explode(' ', $divide_info)[1];
                $divide_info_sta = explode(' ', $divide_info)[2];
                $divide_info_rs  = explode(' ', $divide_info)[3];

                $divide_info_rs  = explode('분', $divide_info_rs)[0];

                if ($divide_info) {
                    $divide_price = 3000;
                    $divide_stand = 1000;
                    $adh_length   = 0;     // 접착 길이

                    if ($divide_info_met == "세로분할") {
                        $adh_length = $vert * ($divide_info_rs - 1);
                    } else {
                        $adh_length = $wid * ($divide_info_rs - 1);
                    }
                    $adh_num   = ceil($adh_length / $divide_stand);
                    $adh_price = $adh_num * $divide_price;

                }

                break;
        }

        // $$ 사용한 파라미터 order_common에 입력할 값으로 변경
        unset($this->fb[$prefix . "sel_cut_wid_size"]);
        unset($this->fb[$prefix . "sel_cut_vert_size"]);
        $this->fb[$prefix . "cut_wid_size"]  = $wid;
        $this->fb[$prefix . "cut_vert_size"] = $vert;

        // 등급할인율
        $temp = [];
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["grade"]         = $this->level;
        $grade_sale_rate = $this->selectGradeSaleRate($temp);

        // $$비교 : 파라미터로 넘어온 등급할인율과 검색결과랑 비교
        if (intval($org_grade_sale_rate) != intval($grade_sale_rate)) {
            $line = __LINE__;
            $str = "등급할인율 불일치";
            $param_val  = $org_grade_sale_rate;
            $search_val = $grade_sale_rate;

            goto ERR;
        }

        // $$비교 : 파라미터로 넘어온 상품가격과 검색결과랑 비교
        $sell_price = $aoUtil->calcPrice($wid, $vert) * $amt;

        if (intval($org_prdt_price) != intval($sell_price)) {
            $line = __LINE__;
            $str = "판매가격 불일치";
            $param_val  = $org_prdt_price;
            $search_val = $sell_price;

            goto ERR;
        }

        // 수량회원할인정보
        unset($temp);
        $temp["member_seqno"]         = $this->member_seqno;
        $temp["cate_sortcode"]        = $cate_sortcode;
        $temp["paper_mpcode"]         = $fb[$prefix . "paper"];
        $temp["bef_print_mpcode"]     = $fb[$prefix . "bef_tmpt"];
        $temp["bef_add_print_mpcode"] = '0';
        $temp["aft_print_mpcode"]     = '0';
        $temp["aft_add_print_mpcode"] = '0';
        $temp["stan_mpcode"]          = $fb[$prefix . "size"];
        $temp["amt"]                  = $amt;
        $amt_sale_rs = $dao->selectAmtMemberCateSale($conn, $temp);
        // $$ 사용한 파라미터 order_common에 입력할 값으로 변경
        $this->fb[$prefix . "size"] = '-1';

        // $$옵션 가격 비교
        $opt_param = [];
        $opt_param["prefix"] = $prefix;
        $opt_param["amt"]    = $amt;

        $opt_ret = $this->validateOptPrice($param);

        if (!$opt_ret["success"]) {
            $line = __LINE__;
            $str = $opt_ret["str"];
            $param_val  = $opt_ret["param"];
            $search_val = $opt_ret["search"];

            goto ERR;
        }
        $opt_price = $opt_ret["search"];
        unset($opt_param);
        unset($opt_ret);

        // $$후공정 가격 비교
        $aft_param = [];
        $aft_param["prefix"] = $prefix;

        $aft_ret = $this->validateAftPrice($aft_param);

        if (!$aft_ret["success"]) {
            $line = __LINE__;
            $str = $aft_ret["str"];
            $param_val  = $aft_ret["param"];
            $search_val = $aft_ret["search"];

            goto ERR;
        }
        $aft_price = $aft_ret["search"];
        unset($aft_param);
        unset($aft_ret);

        $sum_price = $sell_price + $opt_price + $aft_price;

        // 할인금액 계산
        $grade_sale = $util->calcPrice($grade_sale_rate, $sell_price);
        $tmp_price = $sell_price + $grade_sale;

        $amt_sale_rate = $amt_sale_rs["rate"];
        $amt_sale_aplc = $amt_sale_rs["aplc_price"];
        $amt_sale_rate = empty($amt_sale_rate) ? 0 : intval($amt_sale_rate);
        $amt_sale_aplc = empty($amt_sale_aplc) ? 0 : intval($amt_sale_aplc);

        $amt_sale = $util->calcPrice($amt_sale_rate, $tmp_price)
                    + $amt_sale_aplc;
        $amt_sale = $util->ceilVal($amt_sale);

        $sale_price  = $sum_price + $grade_sale + $amt_sale;
        $sale_price += $adh_price;

        // $$비교 : 파라미터로 넘어온 판매가격와 계산된 판매가격 비교
        if (intval($sale_price) !== intval($org_sale_price)) {
            $line = __LINE__;
            $str = "총 판매가격 불일치(할인적용)";
            $param_val  = $sale_price;
            $search_val = $org_sale_price;

            goto ERR;
        }

        return true;

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return false;
    }

    /**
     * @brief 실사제품 배너거치대 가격 확인
     *
     * @param $param = 구분별 공통데이터
     */
    private function validateAoRackPrice($param): bool {
        // 에러처리용 변수
        $line       = null;
        $str        = null;
        $param_val  = null; 
        $search_val = null;

        $prefix = $param["prefix"];

        $opt_param = [];
        $opt_param["prefix"] = $prefix;

        $opt_ret = $this->validateOptPrice($opt_param);

        if (!$opt_ret["success"]) {
            $line = __LINE__;
            $str = $opt_ret["str"];
            $param_val  = $opt_ret["param"];
            $search_val = $opt_ret["search"];

            goto ERR;
        }

        return true;

        ERR:
            $msg = sprintf($this->msg_ptrn, $line
                                          , $this->member_seqno
                                          , $str
                                          , $cate_sortcode
                                          , $param_val
                                          , $search_val);
            $this->setMsg($msg);
            echo $msg;

            return false;
    }

    /**
     * @brief 옵션 가격 확인
     *
     * @param 일부 공통데이터
     */
    private function validateOptPrice($param): array {
        $success    = true;
        $str        = null;
        $param_val  = null;
        $search_val = null;

        $conn    = $this->conn;
        $util    = $this->util;
        $optUtil = $this->optUtil;
        $aoUtil  = $this->aoUtil;

        $prefix        = $param["prefix"];
        $cate_sortcode = $this->fb[$prefix . "cate_sortcode"];
        $sortcode_t    = substr($cate_sortcode, 0, 3);

        $mpcode_arr = explode('|', $this->fb["opt_add"]);
        $price_arr  = explode('|', $this->fb["opt_add_price"]);
        $info_arr   = explode('|', $this->fb["opt_add_info"]);

        $mpcode_arr_count = count($mpcode_arr);

        $temp = [];
        if ($sortcode_t === "002") {
            // $$실사출력
            $dao  = $this->aoDAO;

            $amt = intval($param["amt"]);

            $sum_org_price = 0;
            $sum_opt_price = 0;

            for ($i = 0; $i < $mpcode_arr_count; $i++) {
                $mpcode    = $mpcode_arr[$i];
                $org_price = $price_arr[$i];
                $info      = $info_arr[$i];

                $temp["mpcode"] = $mpcode;

                $rs = $dao->selectAoOptInfoByMpcode($conn, $temp);
                $opt_name  = $rs["opt_name"];
                $unitprice = intval($rs["unitprice"]);

                if ($opt_name === "추가물품" || $opt_name === "거치대") {
                    // info가 개수
                    $amt = intval($info);
                }

                if ($opt_name === "거치대") {
                    unset($temp);
                    $temp["depth1"]    = $rs["depth1"];
                    $temp["depth2"]    = $rs["depth2"];
                    $temp["unitprice"] = $rs["unitprice"];
                    $temp["amt"]       = $amt;

                    $price_ret = $aoUtil->calcRackPrice($temp);
                    $price = $price_ret["price"];

                } else {
                    $price = $unitprice * $amt;
                }

                $price *= 1.1;
                $price  = $util->ceilVal($price);

                if (intval($org_price) !== intval($price)) {
                    $success = false;
                    $str = "옵션 가격 불일치(mpcode : " . $mpcode . ')';
                    $param_val  = $org_price;
                    $search_val = $price;
                    break;
                }

                $sum_org_price += intval($org_price);
                $sum_opt_price += intval($price);
            }
        } else {
            $sell_price    = $param["sell_price"];
            $paper_mpcode  = $param["paper_mpcode"];
            $affil         = $param["size_affil"];
            $expect_box    = $param["expect_box"];
            $count         = intval($this->fb[$prefix . "count"]);

            // 종이정보 검색
            $paper_info = $this->dao->selectCatePaperInfo($conn,
                                                          $paper_mpcode);
            $paper_info = $util->makePaperInfoStr($paper_info, null);

            $temp["cate_sortcode"] = $cate_sortcode;
            $temp["amt"]           = $this->fb[$prefix . "amt"];
            $temp["paper_mpcode"]  = $paper_mpcode;

            $temp["paper_info"]    = $paper_info;
            $temp["affil"]         = $affil;

            $temp["sell_price"]    = $sell_price;
            $temp["expect_box"]    = $expect_box;

            $sum_org_price = 0;
            $sum_opt_price = 0;

            for ($i = 0; $i < $mpcode_arr_count; $i++) {
                $mpcode     = $mpcode_arr[$i];
                $org_price  = $price_arr[$i];

                $temp["mpcode"] = $mpcode;

                $opt_price = $optUtil->calcOptPrice($temp);

                // 180111 이청산 :  카운트 주석처리
                //if (intval($org_price) !== intval($opt_price["price"] * $count)) {
                if (intval($org_price) !== intval($opt_price["price"])) {
                    $success = false;
                    $str = "옵션 가격 불일치??(mpcode : " . $mpcode . ')';
                    $param_val  = $org_price;
                    $search_val = $opt_price["price"];
                    break;
                }

                $sum_org_price += intval($org_price);
                $sum_opt_price += intval($opt_price["price"]);
            }

        }

        if ($success) {
            $param_val  = $sum_org_price;
            $search_val = $sum_opt_price;
        }

        return [
            "success" => $success,
            "str"     => $str,
            "param"   => $param_val,
            "search"  => $search_val
        ];
    }

    /**
     * @brief 후공정 가격 확인
     *
     * @param 일부 공통데이터
     */
    private function validateAftPrice($param) {
        $success    = true;
        $str        = null;
        $param_val  = null;
        $search_val = null;

        $conn    = $this->conn;
        $fb      = $this->fb;
        $util    = $this->util;

        $prefix = $param["prefix"];
        $cate_sortcode = $fb[$prefix . "cate_sortcode"];
        $sortcode_t    = substr($cate_sortcode, 0, 3);
        $amt           = $fb[$prefix . "amt"];
        $amt_unit      = $fb[$prefix . "amt_unit"];
        $count         = intval($fb[$prefix . "count"]);
        $size_name     = $param["size_name"];

        $aft_en_arr = ProductInfoClass::AFTER_ARR;

        if (empty($count)) {
            $count = 1;
        }

        $temp = [];
        if ($sortcode_t === "002") {
            // $$실사출력
            $aoUtil = $this->aoUtil;
            $dao    = $this->aoDAO;

            $wid  = $fb[$prefix . "cut_wid_size"];
            $vert = $fb[$prefix . "cut_vert_size"];

            $aft_ko_arr = $fb[$prefix . "chk_after"];
            $aft_ko_arr_count = count($aft_ko_arr);

            $sum_org_price = 0;
            $sum_aft_price = 0;

            for ($i = 0; $i < $aft_ko_arr_count; $i++) {
                $price = 0;

                $aft_ko = $aft_ko_arr[$i];
                $aft_en = $aft_en_arr[$aft_ko];

                $mpcode    = $fb[$prefix . $aft_en . "_val"];
                $org_price = $fb[$prefix . $aft_en . "_price"];
                $info      = $fb[$prefix . $aft_en . "_info"];

                $temp["mpcode"] = $mpcode;

                $rs = $dao->selectAoAfterInfoByMpcode($conn, $temp);
                $unitprice = intval($rs["unitprice"]);

                // 미싱류 위치
                $t_pos = -1;
                $b_pos = -1;
                $l_pos = -1;
                $r_pos = -1;
                // 고리류 위치별 개수
                $t_cnt = 0;
                $b_cnt = 0;
                $l_cnt = 0;
                $r_cnt = 0;

                $info_arr = [];
                if (!empty($info)) {
                    $t_pos = 0;
                    $b_pos = 0;
                    $l_pos = 0;
                    $r_pos = 0;

                    $info_arr = explode('/', $info);
                }

                if ($aft_ko === "고리") {
                    foreach ($info_arr as $val) {
                        $val_arr = explode('_', $val);
                        $pos = $val_arr[0];
                        $cnt = $val_arr[1];

                        switch ($pos) {
                            case "상" :
                                $t_cnt = $cnt;
                                break;
                            case "하" :
                                $b_cnt = $cnt;
                                break;
                            case "좌" :
                                $l_cnt = $cnt;
                                break;
                            case "우" :
                                $r_cnt = $cnt;
                                break;
                        }
                    }

                } else {
                    foreach ($info_arr as $val) {
                        switch ($val) {
                            case "상" :
                                $t_pos = 1;
                                break;
                            case "하" :
                                $b_pos = 1;
                                break;
                            case "좌" :
                                $l_pos = 1;
                                break;
                            case "우" :
                                $r_pos = 1;
                                break;
                        }
                    }
                }

                unset($temp);
                $temp["unitprice"] = $unitprice;
                $temp["pos_arr"] = [
                     "t" => $t_pos
                    ,"b" => $b_pos
                    ,"l" => $l_pos
                    ,"r" => $r_pos
                ];
                $temp["cnt_arr"] = [
                     "t" => $t_cnt
                    ,"b" => $b_cnt
                    ,"l" => $l_cnt
                    ,"r" => $r_cnt
                ];
                $temp["amt"] = $amt;
                $temp["wid"] = $wid;
                $temp["vert"] = $vert;

                if ($cate_sortcode === "002005001" && $aft_ko === "미싱") {
                    //$$ 게릴라 현수막이면서 미싱이면 값 0원으로 처리
                    $price = 0;

                    //$$ 게릴라 현수막 미상은 좌우 고정
                    if ($l_pos <= 0 || $r_pos <= 0) {
                        $success = false;
                        $str = "후공정(" . $aft_ko . ") 항목 불일치(mpcode : " . $mpcode . ')';
                        $param_val  = $t_pos . '/' . $b_pos . '/'
                                      . $l_pos . '/' . $r_pos;
                        $search_val = $info;

                        break;
                    }
                } else {
                    $price  = $aoUtil->calcAfterPrice($temp);
                    $price *= 1.1;
                    $price  = $util->ceilVal($price);;
                }

                if (intval($org_price) !== intval($price)) {
                    $success = false;
                    $str = "후공정(" . $aft_ko . ") 가격 불일치(mpcode : " . $mpcode . ')';
                    $param_val  = $org_price;
                    $search_val = $price * $count;

                    break;
                }

                $sum_org_price += intval($org_price);
                $sum_aft_price += intval($price);
            }

            if ($success) {
                $param_val  = $sum_org_price;
                $search_val = $sum_aft_price;
            }
        } else {
            // 그 외
            $dao     = $this->dao;
            $aftUtil = $this->aftUtil;
            $aftBindingUtil = $this->aftBindingUtil;

            $pos_arr = PrdtDefaultInfo::POSITION_NUMBER[$cate_sortcode];

            $aft_ko_arr = $fb[$prefix . "chk_after"];
            $aft_ko_arr_count = count($aft_ko_arr);

            $sum_org_price = 0;
            $sum_aft_price = 0;

            for ($i = 0; $i < $aft_ko_arr_count; $i++) {
                $aft_ko = $aft_ko_arr[$i];
                $aft_en = $aft_en_arr[$aft_ko];

                $mpcode    = $fb[$prefix . $aft_en . "_val"];
                $org_price = $fb[$prefix . $aft_en . "_price"];

                $temp["amt"] = $amt;

                if ($aft_ko === '박'
                        || $aft_ko === "형압"
                        || $aft_ko === "엠보싱") {

                    if ($sortcode_t === "001") {
                        $tmp = [
                             "amt"       => $amt
                            ,"pos_num"   => $pos_arr[$size_name]
                            ,"page_num"  => $param["page"]
                            ,"amt_unit"  => null
                            ,"crtr_unit" => "장"
                        ];

                        $real_amt = $util->getPaperRealPrintAmt($tmp);
                        $amt = $real_amt;
                    }

                    $tmp = [
                        "cate_sortcode" => $cate_sortcode,
                        "aft"           => $aft_en,
                        "amt"           => $amt,
                        "sheet_count"   => $fb[$prefix . "sheet_count"],
                        "aft_1"         => $fb[$prefix . $aft_en . "_1"],
                        "dvs_1"         => $fb[$prefix . $aft_en . "_dvs_1"],
                        "aft_2"         => $fb[$prefix . $aft_en . "_2"],
                        "dvs_2"         => $fb[$prefix . $aft_en . "_dvs_2"],
                        "wid_1"         => $fb[$prefix . $aft_en . "_wid_1"],
                        "vert_1"        => $fb[$prefix . $aft_en . "_vert_1"],
                        "wid_2"         => $fb[$prefix . $aft_en . "_wid_2"],
                        "vert_2"        => $fb[$prefix . $aft_en . "_vert_2"]
                    ];

                    $aft_price = $aftUtil->getAfterFoilPressPrice($tmp)["price"];
                } else if ($sortcode_t === "001" && $aft_ko === "제본") {
                    $temp["mpcode"]    = $mpcode;
                    $binding_price = $dao->selectCateAfterPrice($conn, $temp)
                                         ->fields["sell_price"];

                    $aftBindingUtil->setData([
                        "cate_sortcode" => $cate_sortcode,
                        "amt"           => $amt,
                        "page"          => $param["page"],
                        "depth1"        => $param["binding_depth1"],
                        "price"         => $binding_price,
                        "stan_name"     => $size_name,
                        "pos_num"       => $pos_arr[$size_name]
                    ]);
                    $aft_price = $aftBindingUtil->calcBindingPrice();
                } else {
                    $temp["mpcode"] = $mpcode;
                    // 라미넥스 일 때 라미넥스 자체 별도수량
                    if ($aft_en === "laminex") {
                        $laminex_amt = intval($fb[$prefix . $aft_en . "_amt"]);
                        $temp["amt"] = $laminex_amt;
                    }
                    // 재단은 명함 빼고 R당 단가 * 수량
                    if ($sortcode_t !== "003" && $aft_en === "cutting") {
                        $temp["amt"] = (intval($amt) > 1) ? 1 : $amt;
                    }

                    $aft_price = $aftUtil->getAfterPrice($temp);

                    if ($sortcode_t !== "003" && $aft_en === "cutting") {
                        $aft_price *= intval($amt);
                    }
                }

                $org_price = $util->ceilVal($org_price);
                $aft_price = $util->ceilVal($aft_price);

                if (intval($org_price) !== intval($aft_price * $count)) {
                    $success = false;
                    $str = "후공정(" . $aft_ko . ") 가격 불일치(mpcode : " . $mpcode . ')';
                    $param_val  = $org_price;
                    $search_val = $aft_price * $count;
                    break;
                }

                $sum_org_price += intval($org_price);
                $sum_aft_price += intval($aft_price);
            }

            if ($success) {
                $param_val  = $sum_org_price;
                $search_val = $sum_aft_price;
            }
        }

        return [
            "success" => $success,
            "str"     => $str,
            "param"   => $param_val,
            "search"  => $search_val
        ];
    }

    /**
     * @brief 비규격 사이즈 검증
     *
     * @param 검증하기위한 파라미터
     *
     * @return 일치하면 true, 불일치하면 false
     */
    private function validateSimilarSize($param): bool {
        $cut_wid  = intval($fb[$prefix . "cut_wid_size"]); 
        $cut_vert = intval($fb[$prefix . "cut_vert_size"]); 

        $temp = [];
        $temp["cate_sortcode"] = $cate_sortcode;
        $temp["cut_wid"]  = $fb[$prefix . "cut_wid_size"];
        $temp["cut_vert"] = $fb[$prefix . "cut_vert_size"];

        $similar_info_arr = $this->nsUtil->getSimilarSizeInfo($temp);

        // @비교 : 파라미터로 넘어온 stan_mpcode와 검색한 값 비교
        if ($param["stan_mpcode"] != $similar_info_arr["mpcode"]) {
            return false;
        }

        return true;
    }

    /**
     * @brief 회원 등급할인율 검색
     *
     * @param 검색용 파라미터
     *
     * @return 회원 등급할인율
     */
    private function selectGradeSaleRate($param) {
        return $this->dao->selectGradeSaleRate($this->conn, $param);
    }

    private function setMsg(string $msg) {
        $this->msg = $msg;
    }

    public function getMsg(): string {
        return $this->msg;
    }
}
