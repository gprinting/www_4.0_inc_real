<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 상품클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 ***********************************************************************************/
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once($_SERVER["INC"] . "/common_dao/ProductCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");

class Product {
	var $name;
	var $sortcode;
	var $dao;
	var $connectionPool;
    var $jarisu = 1;
	var $util;
	var $conn;
	var $order_common_param;
    var $order_detail_param;
    var $order_dlvr_param;
    var $order_file_param;
    var $order_after_history_param;
    var $order_option_history_param;
    var $member_seqno;
    var $discount_rate = 0;

	function __construct($sortcode) {
		$this->sortcode = $sortcode;
		$this->connectionPool = new ConnectionPool();
		$this->conn = $this->connectionPool->getPooledConnection();
		$this->dao = new ProductCommonDAO();
		$this->util = new FrontCommonUtil();
		$this->name = "";
	}


    function makeOrderCommonInsertParam($fb, $session) {
        $frontUtil = new FrontCommonUtil();
	    $this->order_common_param = array();
        if($fb["order_common_seqno"] != '') {
            $this->order_common_param["order_common_seqno"] = $fb["order_common_seqno"];
            $order_common_seqno = $fb["order_common_seqno"];
        }
        $this->order_common_param["group_seqno"] = 0;
        $this->order_common_param["del_yn"] = 'N';
        $this->order_common_param["count"] = $fb["count"];
        $this->order_common_param["title"] = $fb["title"];
        $this->order_common_param["cust_memo"] = $fb["cust_memo"];
        $this->order_common_param["member_seqno"] = $session["org_member_seqno"];
        $this->order_common_param["order_state"] = "1120";
        $this->order_common_param["claim_yn"] = 'N';
        $this->order_common_param["point_use_yn"] = 'N';
        $this->order_common_param["oper_sys"] = $fb["oper_sys"];
        $this->order_common_param["file_upload_dvs"] = 'Y';
        $this->order_common_param["add_opt_price"] = $frontUtil->rmComma($fb["opt_price"]);
        $this->order_common_param["event_price"] = $fb["event_price"];
        $this->order_common_param["event_yn"] = $fb["event_yn"];
        $this->order_common_param["dlvr_dvs"] = $this->getSort();
        if($this->order_common_param["event_yn"] == null) $this->order_common_param["event_yn"] = "N";
        $common_prdt_dvs = $fb["prdt_dvs"];
        if (!empty($common_prdt_dvs)) {
            $prefix = $common_prdt_dvs . '_';

            $common_cate_sortcode = $fb[$prefix . "cate_sortcode"];
            $common_sell_price = $fb[$prefix . "sell_price"];
            $common_sale_price = $fb[$prefix . "sale_price"];
            $common_amt        = $fb[$prefix . "amt"];
            $common_count        = $fb[$prefix . "count"];
            if($common_count == "") {
                $common_count = "1";
            }

            $this->connectionPool = new ConnectionPool();
            $this->conn = $this->connectionPool->getPooledConnection();
            $this->dao = new ProductCommonDAO();
            $cate_info = $this->dao->selectCateInfo($this->conn, $common_cate_sortcode);
            $flattyp_yn   = $cate_info["flattyp_yn"];
            $tmpt_dvs     = $cate_info["tmpt_dvs"];
            $amt_unit_dvs = $cate_info["amt_unit"];
            unset($cate_info);
            $this->order_detail_param['tmpt_dvs'] = $tmpt_dvs;
            $this->order_detail_param['flattyp_yn'] = $flattyp_yn;
            $this->order_common_param["cate_sortcode"] = $common_cate_sortcode;

            $this->order_common_param["mono_yn"]       = 'N';
            $this->order_common_param["amt"]           = $common_amt;
            $this->order_common_param["amt_unit_dvs"]  = $amt_unit_dvs;
            $this->order_common_param["count"]         = $common_count;

            if (empty($fb[$prefix . "sheet_count"]) === true) {
                $amt = doubleval($fb[$prefix . "amt"]);
            } else {
                $amt = doubleval($fb[$prefix . "sheet_count"]);
            }
            $this->order_common_param["page_cnt"] = $amt;
            // 주문_번호
            $order_num = $frontUtil->makeOrderNum($this->conn, $this->dao,
                $common_cate_sortcode);
            $this->order_common_param["order_num"] = $order_num;

            // 공통 후공정 가격(제본비 등)
            $common_after_price = $fb[$prefix . "after_price"];

            // 책자 제본 정보
            $binding_mpcode = $fb[$prefix . "binding_val"];

            $cate_name         = $fb[$prefix . "cate_name"]; //1
            $size_name         = $fb[$prefix . "size_name"]; // 3
            $paper_name        = $fb[$prefix . "paper_name"]; // 2
            $bef_tmpt_name     = $fb[$prefix . "bef_tmpt_name"];
            $aft_tmpt_name     = $fb[$prefix . "aft_tmpt_name"];
            $bef_add_tmpt_name = $fb[$prefix . "bef_add_tmpt_name"];
            $aft_add_tmpt_name = $fb[$prefix . "aft_add_tmpt_name"];

            $tmpt_str = '';
            if ($tmpt_dvs === '0') {
                $tmpt_str = $bef_tmpt_name;
            } else {
                $tmpt_str = "전면 : " . $bef_tmpt_name;
                if (!empty($bef_add_tmpt_name)) {
                    $tmpt_str .= ", 전면추가 : " . $bef_add_tmpt_name;
                }
                if (!empty($aft_tmpt_name)) {
                    $tmpt_str .= ", 후면 : " . $aft_tmpt_name;
                }
                if (!empty($aft_add_tmpt_name)) {
                    $tmpt_str .= ", 후면추가 : " . $aft_add_tmpt_name;
                }
            }

            $param["fb"]       = $fb;
            $param["prefix"]       = $prefix;
            $expec_weight = $frontUtil->calcExpectWeight($this->conn, $this->dao, $param);
            $this->order_common_param["expec_weight"] = $expec_weight;
        }

        $this->order_common_param["order_detail"] = $fb[$prefix . "order_detail"];
        $this->order_common_param["opt_use_yn"] = 'Y';
        if (empty($fb["opt_add"]) === true) {
            $this->order_common_param["opt_use_yn"] = 'N';
        }

        $this->order_common_param["owncompany_img_num"] = '';
        if (empty($fb["owncompany_img_num"]) === true) {
            $this->order_common_param["owncompany_img_num"] = '';
        } else {
            $this->order_common_param["owncompany_img_num"] = $fb["owncompany_img_num"];
        }

        $this->order_common_param["dlvr_produce_dvs"] = "지방판";

        //$this->order_common_param["sell_price"] = $price["sell_price"];
        //$this->order_common_param["pay_price"] = $price["sell_price"];
        $this->order_common_param["grade_sale_price"] = 0;
        $this->order_common_param["use_point_price"] = 0;
        $this->order_common_param["event_price"] = 0;
        $this->order_common_param["member_sale_price"] = 0;
        $this->order_common_param["receipt_dvs"] = "Manual";
        $this->order_common_param["bun_group"] = "to_1";
        $this->order_common_param["bun_yn"] = "N";
        $this->order_common_param["add_after_price"]  = $price["add_after_price"];
        if($this->order_common_param["add_after_price"] == "") {
            $this->order_common_param["add_after_price"] = 0;
        }
        $this->order_common_param["commerce_dvs"]  = "일반"; // 일반, 상업
    }

    function makeOrderDetailInsertParam($fb, $session) {
        //$this->order_detail_param = array();
        $this->order_detail_param["order_common_seqno"] = $this->order_common_param["order_common_seqno"];
        $this->order_detail_param["cate_sortcode"] = $this->order_common_param["cate_sortcode"];
        $this->order_detail_param["order_detail_dvs_num"] = "S" . $this->order_common_param["order_num"] . "01";
        $this->order_detail_param["state"] = "1220";
        $this->order_detail_param["typ"] = "표지";
        $this->order_detail_param["sell_price"] = $this->order_common_param["sell_price"];
        $this->order_detail_param["grade_sale_price"] = 0;
        $this->order_detail_param["member_sale_price"] = 0;
        $this->order_detail_param["use_point_price"] = 0;
        $this->order_detail_param["pay_price"] = $this->order_common_param["sell_price"];
        $this->order_detail_param["del_yn"] = "N";
        $this->order_detail_param["amt"] = $this->order_common_param["amt"];
        $this->order_detail_param["count"] = $this->order_common_param["count"];

        $common_prdt_dvs = $fb["prdt_dvs"];

        if (!empty($common_prdt_dvs)) {
            $prefix = $common_prdt_dvs . '_';

            $this->order_detail_param["work_size_wid"] = $fb[$prefix . "work_wid_size"];
            $this->order_detail_param["work_size_vert"] = $fb[$prefix . "work_vert_size"];
            $this->order_detail_param["cut_size_wid"] = $fb[$prefix . "cut_wid_size"];
            $this->order_detail_param["cut_size_vert"] = $fb[$prefix . "cut_vert_size"];
            $this->order_detail_param["receipt_size_wid"] = $this->order_detail_param["work_wid_size"];
            $this->order_detail_param["receipt_size_vert"] = $this->order_detail_param["work_vert_size"];
            $this->order_detail_param["tomson_size_wid"] = $fb[$prefix . "tomson_size_wid"];
            $this->order_detail_param["tomson_size_vert"] = $fb[$prefix . "tomson_size_vert"];

            $this->order_detail_param["cate_paper_mpcode"] = $fb[$prefix . "paper"];
            $this->order_detail_param["cate_paper_tot_mpcode"] = $fb[$prefix . "paper"];
            if (!empty($fb[$prefix . "paper_list"])) {
                $this->order_detail_param["cate_paper_tot_mpcode"] = $fb[$prefix . "paper_list"];
            }

            $this->order_detail_param["cate_bef_print_mpcode"] = intval($fb[$prefix . "bef_tmpt"]);
            $this->order_detail_param["cate_aft_print_mpcode"] = intval($fb[$prefix . "aft_tmpt"]);
            $this->order_detail_param["cate_bef_add_print_mpcode"] = intval($fb[$prefix . "bef_add_tmpt"]);
            $this->order_detail_param["cate_aft_add_print_mpcode"] = intval($fb[$prefix . "aft_add_tmpt"]);
            $this->order_detail_param["cate_output_mpcode"] = $fb[$prefix . "size"];

            $this->order_detail_param["order_detail"] = $this->order_common_param["order_detail"];
            $this->order_detail_param["mono_yn"] = "N";

            $size_dvs = $fb[$prefix . "size_dvs"];
            if ($size_dvs === "manu") {
                $stan_name = "비규격";
            } else {
                $stan_name = $fb[$prefix . "size_name"];
            }

            $this->order_detail_param["stan_name"] = $stan_name;
            $this->order_detail_param["print_purp_dvs"] = $fb[$prefix . "print_purp"];
            $tot_tmpt = 0;
            // 단면/양면 구분값
            $side_dvs = null;
            if (empty($fb[$prefix . "tot_tmpt"]) === true) {
                $tot_tmpt = $fb[$prefix . "tot_tmpt"];
            }

            $param = array();
            $param["typ"]        = "표지";
            $param["tmpt_dvs"]   = $this->order_detail_param["tmpt_dvs"];
            $param["flattyp_yn"] = $this->order_detail_param["flattyp_yn"];
            $param["util"]       = new FrontCommonUtil();
            $param["fb"]       = $fb;
            $tmpt_name = $this->makePrintTmptName($this->conn,
                $this->dao,
                $param,
                $tot_tmpt,
                $side_dvs);
            $this->order_detail_param["expec_weight"] = $this->order_common_param["expec_weight"];
            $this->order_detail_param["print_tmpt_name"] = $tmpt_name;
            $this->order_detail_param["tot_tmpt"] = $tot_tmpt;
            $this->order_detail_param["page_amt"] = $fb[$prefix . "page"];
            if (!$this->order_detail_param["page_amt"]) {
                $this->order_detail_param["page_amt"] = 2;
            }
            $this->order_detail_param["amt_unit_dvs"] = $this->order_common_param["amt_unit_dvs"];

            $this->order_detail_param["side_dvs"] = $side_dvs;
            $this->order_detail_param["tomson_yn"] = "N";
            $this->order_detail_param["typset_way"] = "MOAMOA";
        }
    }

    function makeOrderDlvrInsertParam($fb) {
        $this->order_dlvr_param = array();
    }

    function makeOrderAfterHistoryInsertParam($fb) {
        $this->getAfterList();
    }

    function makeOrderOptionHistoryInsertParam($fb) {
        $this->getOptionList();
    }


    function makeOrderFileInsertParam($fb) {
        $this->order_file_param = array();
    }

    function makeBizhowsOrderCommonInsertParam($fb) {
        $frontUtil = new FrontCommonUtil();
        $this->order_common_param = array();
        $this->order_common_param["group_seqno"] = 0;
        $this->order_common_param["del_yn"] = 'N';
        $this->order_common_param["title"] = $fb["Subject"];
        $this->order_common_param["cust_memo"] = $fb["Memo"];
        //$this->order_common_param["member_seqno"] = 6178;
        $this->order_common_param["member_seqno"] = 1976;
        $this->order_common_param["order_state"] = "1320";
        $this->order_common_param["claim_yn"] = 'N';
        $this->order_common_param["point_use_yn"] = 'N';
        $this->order_common_param["bun_group"] = 'to_1';
        $this->order_common_param["oper_sys"] = "IBM";
        $this->order_common_param["file_upload_dvs"] = 'Y';
        $this->order_common_param["add_opt_price"] = 0;
        $this->order_common_param["event_price"] = 0;
        $this->order_common_param["event_yn"] = "N";

        $common_cate_sortcode = $fb["cate_sortcode"];
        $common_amt        = $fb["Qty"];
        $common_count        = $fb["Num"];

        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new ProductCommonDAO();

        $cate_info = $this->dao->selectCateInfo($this->conn, $common_cate_sortcode);
        $flattyp_yn   = $cate_info["flattyp_yn"];
        $tmpt_dvs     = $cate_info["tmpt_dvs"];
        $amt_unit_dvs = $cate_info["amt_unit"];
        unset($cate_info);
        $this->order_detail_param['tmpt_dvs'] = $tmpt_dvs;
        $this->order_detail_param['flattyp_yn'] = $flattyp_yn;
        $this->order_common_param["cate_sortcode"] = $common_cate_sortcode;

        $this->order_common_param["mono_yn"]       = 'N';

        $this->order_common_param["amt"]           = $common_amt;
        $this->order_common_param["amt_unit_dvs"]  = $amt_unit_dvs;
        $this->order_common_param["count"]         = $common_count;

        $this->order_common_param["page_cnt"] = $fb["PageCnt"];
        // 주문_번호
        $order_num = $frontUtil->makeBizhowsOrderNum($this->conn, $this->dao,
            $common_cate_sortcode);
        $this->order_common_param["order_num"] = $order_num;

        // 공통 후공정 가격(제본비 등)
        $common_after_price = $fb["After_TPrice"];

        $this->order_common_param["expec_weight"] = $fb["ExpecWeight"];
        $this->order_common_param['pay_way'] = "선입금";
        $this->order_common_param['depo_finish_date'] = date("Y-m-d G:i:s");

        $this->order_common_param["opt_use_yn"] = 'N';
        $this->order_common_param["owncompany_img_num"] = '';

        $this->order_common_param["dlvr_produce_dvs"] = "지방판";

        $this->order_common_param["sell_price"] = $fb["Price"];
        $this->order_common_param["pay_price"] = $fb["Price"];
        $this->order_common_param["grade_sale_price"] = 0;
        $this->order_common_param["use_point_price"] = 0;
        $this->order_common_param["event_price"] = 0;
        $this->order_common_param["member_sale_price"] = 0;
        $this->order_common_param["receipt_dvs"] = "Manual";
        $this->order_common_param["bun_group"] = "to_1";
        $this->order_common_param["bun_yn"] = "N";
        $this->order_common_param["point_use_yn"] = "N";
        $this->order_common_param["add_after_price"]  = $common_after_price;

        $this->order_common_param["commerce_dvs"]  = "일반"; // 일반, 상업
        $this->order_common_param["dlvr_dvs"]  = "namecard";

    }

    function makeBizhowsOrderDetailInsertParam($fb) {
        //$this->order_detail_param = array();
        $this->order_detail_param["order_common_seqno"] = $this->order_common_param["order_common_seqno"];
        $this->order_detail_param["cate_sortcode"] = $this->order_common_param["cate_sortcode"];
        $this->order_detail_param["order_detail_dvs_num"] = "S" . $this->order_common_param["order_num"] . "01";
        $this->order_detail_param["state"] = "1320";
        $this->order_detail_param["typ"] = "표지";
        $this->order_detail_param["sell_price"] = $this->order_common_param["sell_price"];
        $this->order_detail_param["grade_sale_price"] = 0;
        $this->order_detail_param["member_sale_price"] = 0;
        $this->order_detail_param["use_point_price"] = 0;
        $this->order_detail_param["pay_price"] = 0;
        $this->order_detail_param["del_yn"] = "N";
        $this->order_detail_param["amt"] = $this->order_common_param["amt"];
        $this->order_detail_param["count"] = $this->order_common_param["count"];


        $this->order_detail_param["work_size_wid"] = $fb["EditSizeW"];
        $this->order_detail_param["work_size_vert"] = $fb["EditSizeH"];
        $this->order_detail_param["cut_size_wid"] = $fb["CutSizeW"];
        $this->order_detail_param["cut_size_vert"] = $fb["CutSizeH"];
        $this->order_detail_param["receipt_size_wid"] = $fb["EditSizeW"];
        $this->order_detail_param["receipt_size_vert"] = $fb["EditSizeH"];
        $this->order_detail_param["tomson_size_wid"] = 0;
        $this->order_detail_param["tomson_size_vert"] = 0;

        $this->order_detail_param["expec_weight"] = $fb["ExpecWeight"];

        $this->order_detail_param["order_detail"] = $this->order_common_param["order_detail"];
        $this->order_detail_param["mono_yn"] = "N";

        $tot_tmpt = 0;
        // 단면/양면 구분값
        $side_dvs = null;
        $param = array();
        $param["typ"]        = "표지";
        $param["tmpt_dvs"]   = $this->order_detail_param["tmpt_dvs"];
        $param["flattyp_yn"] = $this->order_detail_param["flattyp_yn"];
        $param["util"]       = new FrontCommonUtil();
        $param["fb"]       = $fb;

        if($fb["Side_Code"] == "S")
            $this->order_detail_param["side_dvs"]  = "단면";
        if($fb["Side_Code"] == "D")
            $this->order_detail_param["side_dvs"]  = "양면";

        $tmpt_name = $this->makePrintTmptName($this->conn,
            $this->dao,
            $param,
            $tot_tmpt,
            $side_dvs);
        //$this->order_detail_param["expec_weight"] = 0;
        $this->order_detail_param["print_tmpt_name"] = $tmpt_name;
        $this->order_detail_param["tot_tmpt"] = $tot_tmpt;
        $this->order_detail_param["page_amt"] = $this->order_detail_param["amt"];
        $this->order_detail_param["amt_unit_dvs"] = $this->order_common_param["amt_unit_dvs"];

        //$this->order_detail_param["side_dvs"] = $side_dvs;
        $this->order_detail_param["tomson_yn"] = "N";
        $this->order_detail_param["typset_way"] = "MOAMOA";
        //$this->size_name = $param["size_name"];

    }

    function makeBizhowsOrderDlvrInsertParam($fb) {
        $this->order_dlvr_param = array();
    }

    function makeBizhowsOrderAfterHistoryInsertParam($fb) {
        $this->order_after_history_param = array();
    }

    public function insertArr( $table, $paramsArr) {
        $sql = "INSERT INTO {$table} (";
        foreach ( $paramsArr as $name=>$value ) {
            $sql .="`$name`,";
        }

        $sql = substr($sql, 0, strlen($sql)-1);
        $sql .= ") VALUES (";
        foreach ($paramsArr as $name => $value){
            $value === null? $sql .= "null," : $sql .= "'$value',";
        }
        $sql = substr($sql, 0, strlen($sql)-1);
        $sql .= ");";

        $this->link->query($sql);
        return $this->link->affected_rows;
    }
/***********************************************************************************
*** 모든 상품의 공통적인 부분, 종이명, 인쇄명, 사이즈, 매수의 정보를 불러옴
*** 인자 number는 페이지물과 같은 여러 종이명, 인쇄명 등이 오는 경우를 대비해서 넣음
***********************************************************************************/

	function makeBasicOption($number) {
		$html = $this->makePaperOption($number);
		$html .= $this-> makePrintOption($number);
		$html .= $this->makeSizeOption($number);
		$html .= $this->makeAmtOption($number);

		return $html;
	}

	function makePaperOption($number) { // 혼합형의 경우 넘버링(number)을 통해 다수의 종이정보를 만들어내는것이 가능하다.
		$price_info_arr = array();
		$param = array();
		$paper = $this->dao->selectCatePaperHtml($this->conn, $this->sortcode, $price_info_arr);

		$html = '종이 : <select id="paper_' . $number . '" name="paper" onchange="changeData();">' . $paper['info'] . '</select></br></br>';

		return $html;
	}

	function makePrintOption($number) {
		$price_info_arr = array();
		$price_info_arr['cate_sortcode'] = $this->sortcode;
		$param = array();
		$param['cate_sortcode'] = $this->sortcode;
		$print_tmpt = $this->dao->selectCatePrintTmptHtml($this->conn, $param, $price_info_arr);
		$print_tmpt = $print_tmpt["단면"] . $print_tmpt["양면"];

		$print_purp = $this->dao->selectCatePrintPurpHtml($this->conn, $this->sortcode);

		$html = '인쇄도수 : <select id="print_' . $number . '" name="print" onchange="changeData();">' . $print_tmpt . '</select></br></br>';
		$html .= '<select id="print_purp_' . $number . '" name="print_purp" style="display:none">' . $print_purp .'</select>';

		return $html;
	}

	function makeSizeOption($number) {
		$price_info_arr = array();
		$price_info_arr['cate_sortcode'] = $this->sortcode;
		$param = array();
		$param['cate_sortcode'] = $this->sortcode;
		$size = $this->dao->selectCateSizeHtml($this->conn, $param, $price_info_arr);

		$html = '사이즈 : <select id="size_' . $number . '" name="size" def_cut_wid='.$price_info_arr['def_cut_wid'].' def_cut_vert=' . $price_info_arr['def_cut_vert'] . ' stan_mpcode=' . $price_info_arr['stan_mpcode'] . '>' . $size . '</select></br></br>';

		return $html;
	}

	function makeAmtOption($number) {
		$price_info_arr = array();
		$price_info_arr['cate_sortcode'] = $this->sortcode;
		$param = array();
		$param['cate_sortcode'] = $this->sortcode;
		$param["table_name"]    = 'ply_price_gp';
		$param["amt_unit"]      = '장';
		$amt = $this->dao->selectCateAmtHtml($this->conn, $param, $price_info_arr);

		$html =  '수량 : <select id="amt_' . $number . '" name="amt" onchange="changeData();">' . $amt . '</select></br></br>';
		return $html;
	}

	function makeOptOption() {
		$opt = $this->dao->selectCateOptHtml($this->conn, $this->sortcode);
		$add_opt = $opt["info_arr"]["name"];
		$add_opt = $this->dao->parameterArrayEscape($this->conn, $add_opt);
		$add_opt = $this->util->arr2delimStr($add_opt);
		$param = array();
		$param["cate_sortcode"] = $this->sortcode;
		$param["opt_name"]      = $add_opt;
		$param["opt_idx"]       = $opt["info_arr"]["idx"];
		$add_opt = $this->dao->selectCateAddOptInfoHtml($this->conn, $param);

		$html = '---------------------옵션-------------------------';
		$html .= '<dd class="_folder list">' . $opt['html'] .$add_opt . '</dd>';
		$html .= '-------------------------------------------------</br></br>';

		return $html;
	}

	function makeAfterOption() {
		$param = array();
		$param["cate_sortcode"] = $this->sortcode;

		$after = $this->dao->selectCateAfterHtml($this->conn, $param);
		$basic_after = $after["info_arr"]["add"];
		$basic_after = $this->dao->parameterArrayEscape($this->conn, $basic_after);
		$basic_after  = $this->util->arr2delimStr($basic_after, ',');

		$param["after_name"]      = $basic_after;

		$add_after = $this->dao->selectCateAddAfterInfoHtml($this->conn, $param);

		$html = '---------------------후공정-------------------------';
		$html .= '<dd class="_folder list">' . $after['html'] .$add_after . '</dd>';
		$html .= '-----------------------------------------------------';

		return $html;
	}

	function makeCommonInfo() {
		$html = '<input type="hidden" id="paper" value="">';
		$html .= '<input type="hidden" id="print" value="">';
		$html .= '<input type="hidden" id="print_purp" value="">';
		$html .= '<input type="hidden" id="size" value="">';
		$html .= '<input type="hidden" id="amt" value="">';
		$html .= '<input type="hidden" id="sortcode" name="sortcode" value="' . $this->sortcode .'"></br>';
		$html .= '<input type="hidden" id="opt_name_list" name="opt_name_list" value="">';
		$html .= '<input type="hidden" id="opt_mp_list" name="opt_mp_list" value="">';
		$html .= '<input type="hidden" id="after_name_list" name="after_name_list" value="">';
		$html .= '<input type="hidden" id="after_mp_list" name="after_mp_list" value="">';
		$html .= '<span id="detail"></span></br></br>';
		$html .= '<span id="total_price">0원</span>';

		return $html;
	}

	function getDescription() {
		return $this->name;
	}


    function setInfo($param)
    {
        $factory = new DPrintingFactory();
        $this->size_w = $param["cut_wid_size"];
        $this->size_h = $param["cut_vert_size"];
        $this->amt = $param["amt"];
        $this->count = $param["count"];
        $this->paper_mpcode = $param["paper"];
        $this->stan_mpcode = $param["size"];
        $this->bef_print_mpcode = $param["bef_tmpt"];
        $this->paper_info = $param["paper_info"];
        $this->member_seqno = $param["member_seqno"];
        //$this->size_name = $param["size_name"];

        $after_name = explode("|", $param["after_name"]);
        $aft_depth = explode("|", $param["aft_depth"]);
        $aft_depth1 = explode("|", $param["aft_depth1"]);
        $aft_depth2 = explode("|", $param["aft_depth2"]);
        $aft_depth3 = explode("|", $param["aft_depth3"]);
        $aft_depth1_val = explode("|", $param["aft_depth1_val"]);
        $aft_depth1_vh = explode("|", $param["aft_depth1_vh"]);
        $aft_depth1_cnt = explode("|", $param["aft_depth1_cnt"]);
        $aft_depth1_dvs = explode("|", $param["aft_depth1_dvs"]);
        $aft_depth2_dvs = explode("|", $param["aft_depth2_dvs"]);
        $aft_depth1_wid = explode("|", $param["aft_depth1_wid"]);
        $aft_depth1_vert = explode("|", $param["aft_depth1_vert"]);
        $aft_depth2_wid = explode("|", $param["aft_depth2_wid"]);
        $aft_depth2_vert = explode("|", $param["aft_depth2_vert"]);
        $aft_depth1_amt = explode("|", $param["aft_depth1_amt"]);

        $option_name = explode("|", $param["option_name"]);
        $opt_depth1 = explode("|", $param["opt_depth1"]);
        $opt_depth2 = explode("|", $param["opt_depth2"]);
        $opt_depth3 = explode("|", $param["opt_depth3"]);
        $opt_mpcode = explode("|", $param["opt_mp_list"]);
        $opt_cnt = explode("|", $param["opt_cnt"]);

        $after_count = count($after_name);
        $product = $this;
        for($i = 0; $i < $after_count ; $i++) {
            if($after_name[$i] == '') {
                break;
            }
            $product = $factory->createAP($after_name[$i], $product);
            $arr_depth = [];
            $arr_depth[$i]['after_name'] = $after_name[$i];
            $arr_depth[$i]['aft_depth'] = $aft_depth[$i];
            $arr_depth[$i]['aft_depth1'] = $aft_depth1[$i];
            $arr_depth[$i]['aft_depth2'] = $aft_depth2[$i];
            $arr_depth[$i]['aft_depth3'] = $aft_depth3[$i];
            $arr_depth[$i]['aft_depth1_val'] = $aft_depth1_val[$i];
            $arr_depth[$i]['aft_depth1_vh'] = $aft_depth1_vh[$i];
            $arr_depth[$i]['aft_depth1_cnt'] = $aft_depth1_cnt[$i];
            $arr_depth[$i]['aft_depth1_dvs'] = $aft_depth1_dvs[$i];
            $arr_depth[$i]['aft_depth2_dvs'] = $aft_depth2_dvs[$i];
            $arr_depth[$i]['aft_depth1_wid'] = $aft_depth1_wid[$i];
            $arr_depth[$i]['aft_depth1_vert'] = $aft_depth1_vert[$i];
            $arr_depth[$i]['aft_depth2_wid'] = $aft_depth2_wid[$i];
            $arr_depth[$i]['aft_depth2_vert'] = $aft_depth2_vert[$i];
            $arr_depth[$i]['aft_depth1_amt'] = $aft_depth1_amt[$i];
            $arr_depth[$i]['size_name'] = $this->size_name;

            $product->setAfterprocess($this->sortcode, $arr_depth[$i], $this->amt); // $sortcode ,$after_name, $mpcode, $amt = ''
        }

        $option_count = count($option_name);
        for($i = 0; $i < $option_count ; $i++) {
            if($option_name[$i] == '') {
                break;
            }
            $product = $factory->createOP($option_name[$i], $product);
            $arr_depth = [];
            $arr_depth[$i]['option_name'] = $option_name[$i];
            $arr_depth[$i]['opt_depth1'] = $opt_depth1[$i];
            $arr_depth[$i]['opt_depth2'] = $opt_depth2[$i];
            $arr_depth[$i]['opt_depth3'] = $opt_depth3[$i];
            $arr_depth[$i]['opt_cnt'] = $opt_cnt[$i];
            $arr_depth[$i]['opt_mp_list'] = $opt_mpcode[$i];
            if($arr_depth[$i]['opt_mp_list'] == '')
                $arr_depth[$i]['opt_mp_list'] = 0;
            $arr_depth[$i]['expect_box'] = $param["expect_box_num"];
            $arr_depth[$i]['affil'] = $param["affil"];
            $arr_depth[$i]['paper_mpcode'] = $param["paper"];
            $arr_depth[$i]['paper_info'] = $param["paper_info"];
            $product->setOption($this->sortcode, $arr_depth[$i], $opt_mpcode[$i], $this->amt); // $sortcode ,$after_name, $mpcode, $amt = ''
        }

        $product = $factory->createDC(null, $product);
        $product->grade = $param[grade];
        return $product;
    }

    public function cost() {}

    public function getProduct() {
        return $this;
    }

    public function getAfterList() {
	    $par = array();
        $par["cate_sortcode"] = $this->sortcode;
        $par["basic_yn"]      = 'Y';
        $default_after_rs = $this->dao->selectCateAfterInfo($this->conn, $par);
        if($this->order_after_history_param == null)
            $this->order_after_history_param = array();
        while ($default_after_rs && !$default_after_rs->EOF) {
            $ret = array();
            $ret['mpcode'] = $default_after_rs->fields["mpcode"];
            $ret['after_name']   = $default_after_rs->fields["after_name"];
            $ret['depth1'] = $default_after_rs->fields["depth1"];
            $ret['depth2'] = $default_after_rs->fields["depth2"];
            $ret['depth3'] = $default_after_rs->fields["depth3"];
            $ret['basic_yn'] = $default_after_rs->fields["basic_yn"];
            array_push($this->order_after_history_param, $ret);
            $default_after_rs->MoveNext();
        }
    }

    public function getOptionList() {
        $par = array();
        $par["cate_sortcode"] = $this->sortcode;
        $par["basic_yn"]      = 'Y';

        $default_option_rs = $this->dao->selectCateOptInfo($this->conn, $par);
        if($this->order_option_history_param == null)
            $this->order_option_history_param = array();
        $param = array();
        while ($default_option_rs && !$default_option_rs->EOF) {
            $ret = array();
            $ret['mpcode'] = $default_option_rs->fields["mpcode"];

            $ret['name']   = $default_option_rs->fields["option_name"];
            $ret['depth1'] = $default_option_rs->fields["depth1"];
            $ret['depth2'] = $default_option_rs->fields["depth2"];
            $ret['depth3'] = $default_option_rs->fields["depth3"];
            array_push($this->order_option_history_param, $ret);
            $default_option_rs->MoveNext();
        }
    }

/***********************************************************************************
*** 인자 $dlvr_way로 배송료를 선택하는 메서드
*** 외부에서 사용 할 수 있도록 반드시 public권한을 주어야함
***********************************************************************************/

	function getDlvrCost($param, $dlvr_way) {
		switch($dlvr_way) {
			case "01" :
				return $this->getParcelCost($param);
				break;
			case "03" :
				return $this->getCargoCost($param);
				break;
			case "04" :
				return $this->getAutobikeCost($param);
				break;
			case "05" :
				return $this->getSubwayCost($param);
				break;
			default :
                $res = array();
                $res["price"] = 0;
                $res["box_count"] = 1;
				return $res;
				break;
		}
	}


/***********************************************************************************
*** 택배사에 맞춰 운임료를 예측하는 함수
*** 주로 이 클래스에서 파생되는 getParcelCost(회사 자체적으로 정해진 운임료 가져옴) 실행 이후 가격을 불러오지 못할경우 실행됨
*** 현재는 무게만 고려하고 있지만 추후 거리나 부피에 따라서도 변동되도록 해야한다
***********************************************************************************/

	function getParcelCost($param) {
		include_once($_SERVER["DOCUMENT_ROOT"] . "/com/nexmotion/job/order/SheetDAO.php");
		$dao = new SheetDAO();
		$rs = $dao->selectParcelCostPrice($this->conn, $param);
        $weight = $param['expec_weight'];
        $part_price = 0;

        if ($param["sort"] === "namecard") {
            if($weight <= 3) {
                $tmp_BoxCount = 1;
                $price_degree = 2750;
            } else if($weight <= 12) {
                $tmp_BoxCount = 1;
                $price_degree = 3300;
            } else {
                $tmp_BoxCount = (int)($weight / 12);
                $part_weight = $weight % 12;
                $price_degree = 3300;
                if($part_weight < 3) {
                    $part_price = 2750;
                } else {
                    $part_price = 3300;
                }
            }

            /*
            if($param["level"] >= 9) {
                if($param["sell_price"] >= 50000)
                    $price_degree = 0;
            } else if($param["level"] >= 6) {
                if($param["sell_price"] >= 40000)
                    $price_degree = 0;
            } else if($param["level"] >= 4) {
                if($param["sell_price"] >= 30000)
                    $price_degree = 0;
            } else if($param["level"] >= 1) {
                if($param["sell_price"] >= 20000)
                    $price_degree = 0;
            }
            */
        // front/ajax22/load_dlvr_price_expect.php 에서 
        //cmt 항목과 catecode를 추가해서 던짐
        //봉투가 별도로 지정이 안되어 있어서 코드로 분리 
        //}else if ($param["sort"] === "leaflet" && $param['catecode'] == "006001003" || $param["sort"] === "leaflet" && $param['catecode'] == "006001001" ) {
        }else if($param["sort"] === "leaflet" && $param['catecode'] == "014005001" ){

            $weight_degree = 17;
            $price_degree = $this->util->bannerBasicPrice($param['cmt2']);
           // $tmp_BoxCount = $this->util->bannerBasicbox($param['cmt2']); 
           $tmp_BoxCount = 1;
        }else if ($param["sort"] === "envelope") {
            $group_a = array("006001003", "006001004", "006001007"); //A4소봉투, 자켓봉투,창봉투
           // $group_a = array("006001003"); //A4소봉투, 자켓봉투,창봉투 * 내일 정상적용할떈 없애기
            $group_b = array("006001001"); // 대봉투
            $group_c = array("006001005"); // 티켓봉투
            $group_c_amt = array("500","1000"); //티켓봉투 수량제한 
            $group_d = array("006001006"); //부동산소봉투
            $group_e = array("006001002"); // 중봉투
            $group_amt = array("500"); // 보통 500매 부터 가격이 다름

            $weight_degree = 17;
            $price_degree = 4950;
           
            if(in_array($param['catecode'], $group_a )){               
                $weight_degree = 9.7;
                $price_degree = 3300;
    
                if($param['cmt'] == "500"){
                    $price_degree = 2750; 
                }
                               
            }
            if(in_array($param['catecode'], $group_b )){   
                $weight_degree = 9.8;
                $price_degree = 3300;                        
            }    
            if(in_array($param['catecode'], $group_c )){   
                $weight_degree = 9.2;
                $price_degree = 3300;
                
                if(in_array($param['cmt'],$group_c_amt)){
                    $price_degree = 2750;
                }     
            }
            if(in_array($param['catecode'], $group_d )){   
                $weight_degree = 9.2;
                $price_degree = 3300;
                
                if(in_array($param['cmt'],$group_amt)){
                    $price_degree = 2750;
                }     
            }   
            
            if(in_array($param['catecode'], $group_e )){   
                $weight_degree = 9.7;
                $price_degree = 3300;                        
            }   
            

            $tmp_BoxCount = (int)($weight / $weight_degree) + 1;
           
        }else{
            $weight_degree = 17;

            if($param['catecode'] == "001005001"){
                $weight_degree = 20;
            }
            $price_degree = 4950;
            if(($param["paper_mpcode"] == "243"
            || $param["paper_mpcode"] == "248") && strpos($param["size"], 'A') !== false) {
                $weight_degree = 12;
                $price_degree = 3300;
            }

            $tmp_BoxCount = (int)($weight / $weight_degree) + 1;
        }

        $res = array();
        $res["price"] = $tmp_BoxCount * $price_degree + $part_price;
        if($part_price != 0) $tmp_BoxCount++;
        if($param["sort"] === "leaflet" && $param['catecode'] == "014005001"){
            $res["box_count"] = $this->util->bannerBasicbox($param['cmt2']);
        }else{
            $res["box_count"] = $tmp_BoxCount;
        }
        
		/*
		$x_cost_per = 2750;
        if ($param["sort"] === "leaflet") {
            $x_cost_per = 3300;
        }

		if($rs->fields['price'] != NULL) {
			$x_cost_per = $rs->fields['price'];
		}

		$weight = $param['expec_weight'];

		$x_degree = 15; // 택배회사에서 덩이를 나누는 기준KG

		$parcel_count = ceil($weight / $x_degree);
		*/
		return $res;
	}


/***********************************************************************************
*** 화물사에 맞춰 운임료를 예측하는 함수
*** 주로 이 클래스에서 파생되는 getCargoCost(회사 자체적으로 정해진 운임료 가져옴) 실행 이후 가격을 불러오지 못할경우 실행됨
*** 현재는 무게만 고려하고 있지만 추후 거리나 부피에 따라서도 변동되도록 해야한다
***********************************************************************************/

	function getCargoCost($param) {
		$weight = $param['expec_weight'];
		$x_degree = 20; //화물사에서 덩이를 나누는 기준KG
		$x_cost_per = 2500; // 화물사에서 받는 덩이당 가격

		$cargo_count = ceil($weight / $x_degree);
        $res = array();
        $res["price"] = $cargo_count * $x_cost_per;
        $res["box_count"] = 1;
		return $res;
	}


/***********************************************************************************
*** 지하철퀵사에 맞춰 운임료를 예측하는 함수
*** 주로 이 클래스에서 파생되는 getSubwayCost(회사 자체적으로 정해진 운임료 가져옴) 실행 이후 가격을 불러오지 못할경우 실행됨
*** 현재는 무게만 고려하고 있지만 추후 거리나 부피에 따라서도 변동되도록 해야한다
***********************************************************************************/

	function getSubwayCost($param) {
		$weight = $param['expec_weight'];
		$x_degree = 20; //지하철퀵사에서 덩이를 나누는 기준KG
		$x_cost_per = 2500; // 지하철퀵사에서 받는 덩이당 가격

		$subway_count = ceil($weight / $x_degree);
        $res = array();
        $res["price"] = $subway_count * $x_cost_per;
        $res["box_count"] = 1;
        return $res;
	}


/*********************************************************************************************************
*** 짐콜퀵사에 맞춰 운임료를 예측하는 함수
*** 주로 이 클래스에서 파생되는 getSubwayCost(회사 자체적으로 정해진 운임료 가져옴) 실행 이후 가격을 불러오지 못할경우 실행됨
*** 현재는 무게만 고려하고 있지만 추후 거리나 부피에 따라서도 변동되도록 해야한다
****************************************************************************************************/

	function getAutobikeCost($param) { // 오토바이퀵사에 맞춰 운임료, 측정 무게에 따라 오토바이, 다마스, 라보로 구분됨
		$weight = $param['expec_weight'];
		$zipcode = $param['zipcode'];
		include_once(INC_PATH . "/com/nexmotion/job/front/order/SheetDAO.inc");
		$dao = new SheetDAO();
		$rs = $dao->selectAutobikeCostPrice($this->conn, $param);
        $price  = 0;
		if(!$rs || $rs->fields['autobike'] == 0) {
			return 0; // 퀵 이용 불가
		} else {
			if ($weight < 21) {
                $price = $rs->fields['autobike']; // 1연 이하 (오토바이)
			} else if ($weight < 42) {
                $price = $rs->fields['autobike'] + 2000; // 2연 이하 (오토바이)
			} else if ($weight < 63) {
                $price = $rs->fields['autobike'] + 5000; // 3연 이하 (오토바이)
			} else if ($weight < 315) {
                $price = $rs->fields['damas']; // 15연 이하 (다마스)
			} else if ($weight < 525) {
                $price = $rs->fields['rabo']; // 25연 이하 (라보)
			} else if ($weight < 900){
                $price = $rs->fields['1ton'];
			} else {
                $price = -1;
			}
		}

        $res = array();
        $res["price"] = $price;
        $res["box_count"] = 1;
        return $res;
	}

    function makePrintTmptName($conn, $dao, $param, &$tot_tmpt, &$side_dvs) {
        $ret = '';
        $tmpt_dvs = $param["tmpt_dvs"];
        if ($tmpt_dvs === '0') {
            $print_mpcode_arr = $this->getPrintMpcodeArr($param);
            $print_mpcode = $print_mpcode_arr["bef"];
            $print_mpcode = $dao->parameterEscape($conn, $print_mpcode);
            $rs = $dao->selectPrintTmptInfo($conn, $print_mpcode);
            $rs = $rs->fields;

            $tot_tmpt = $rs["tot_tmpt"];

            if (!empty($rs["beforeside_tmpt"])) {
                $side_dvs = "단면";
            }

            if (!empty($rs["aftside_tmpt"])) {
                $side_dvs = "양면";
            }

            $ret .= sprintf("전면 %s / 후면 %s / 추가 %s", $rs["beforeside_tmpt"]
                , $rs["aftside_tmpt"]
                , $rs["add_tmpt"]);

        } else {
            $print_mpcode_arr = $this->getPrintMpcodeArr($param);
            $print_mpcode = $dao->arr2paramStr($conn, $print_mpcode_arr);
            $rs = $dao->selectPrintTmptInfo($conn, $print_mpcode);

            $ret = $this->getPrintTmptInfo($rs, $tot_tmpt, $side_dvs);
        }

        return $ret;
    }

    function getPrintMpcodeArr($param) {
        $tmpt_dvs = $param["tmpt_dvs"];
        $prefix   = $param["prefix"];
        $fb       = $param["fb"];

        return [
            "bef" => intval($fb[$prefix . "bef_tmpt"]),
            "aft" => intval($fb[$prefix . "aft_tmpt"]),
            "bef_add" => intval($fb[$prefix . "bef_add_tmpt"]),
            "aft_add" => intval($fb[$prefix . "aft_add_tmpt"])
        ];
    }

    function startsWith($haystack, $needle) {
        // search backwards starting from haystack length characters from the end
        return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== false;
    }

    /**
     * @brief makePrintTmptName() 에서 사용하는 함수
     * 쿼리 검색결과에서 면구분/도수 값만 추출해서 문자열 생성
     *
     * @param $rs        = 쿼리 검색결과
     * @param &$tot_tmpt = 총도수 반환용 변수
     * @param &$side_dvs = 단/양면 구분값 반환용 변수
     *
     * @return $면구분$/$도수$ 문자열
     */
    function getPrintTmptInfo($rs, &$tot_tmpt, &$side_dvs) {
        $ret = '';

        $bef_form = "전면 %s + %s";
        $aft_form = "후면 %s + %s";
        $bef_add_form = "전면추가 %s + %s";
        $aft_add_form = "후면추가 %s + %s";

        $bef = '';
        $aft = '';
        $bef_add = '';
        $aft_add = '';

        $bef_chk = false;
        $aft_chk = false;

        while ($rs && !$rs->EOF) {
            $side_dvs = $rs->fields["side_dvs"];
            $add_tmpt = $rs->fields["add_tmpt"];

            $tot_tmpt += intval($rs->fields["tot_tmpt"]);

            if ($side_dvs === "전면") {
                $tmpt = $rs->fields["beforeside_tmpt"];

                if (intval($tmpt) > 0 || intval($add_tmpt) > 0) {
                    $bef_chk = true;
                }

                $bef = sprintf($bef_form, $tmpt, $add_tmpt);
            } else if ($side_dvs === "후면") {
                $tmpt = $rs->fields["aftside_tmpt"];

                if (intval($tmpt) > 0 || intval($add_tmpt) > 0) {
                    $aft_chk = true;
                }

                $aft = sprintf($aft_form, $tmpt, $add_tmpt);
            } else if ($side_dvs === "전면추가") {
                $tmpt = $rs->fields["beforeside_tmpt"];

                if (intval($tmpt) > 0 || intval($add_tmpt) > 0) {
                    $bef_chk = true;
                }

                $bef_add = sprintf($bef_add_form, $tmpt, $add_tmpt);
            } else if ($side_dvs === "후면추가") {
                $tmpt = $rs->fields["aftside_tmpt"];

                if (intval($tmpt) > 0 || intval($add_tmpt) > 0) {
                    $aft_chk = true;
                }

                $aft_add = sprintf($aft_add_form, $tmpt, $add_tmpt);
            }

            $ret .= sprintf($form, $tmpt, $add_tmpt);

            $rs->MoveNext();
        }

        if ($bef_chk || $aft_chk) {
            $side_dvs = "단면";
        }

        if ($bef_chk && $aft_chk) {
            $side_dvs = "양면";
        }

        $ret = $bef;
        if (!empty($bef_add)) {
            $ret .= " / " . $bef_add;
        }
        $ret .= " / " . $aft;
        if (!empty($aft_add)) {
            $ret .= " / " . $aft_add;
        }

        return $ret;
    }


    //프린파이 인쇄견적 후가공 파라미터 생성
    //2024.12.04. 권혁서 ( louiskwon@gmail.com)
    function makePrintfyEstimateOrderAfterHistoryInsertParam($fb) {

        $this->order_after_history_param = array();

        $arr_after_code = explode("|", $fb['after_code']);
        $arr_after_name = explode("|", $fb['after_name']);
        $arr_after_depth1 = explode("|", $fb['after_depth1']);
        $arr_after_depth2 = explode("|", $fb['after_depth2']);
        $arr_after_depth3 = explode("|", $fb['after_depth3']);
        $arr_after_detail = explode("|", $fb['after_detail']);
        $arr_after_info = explode("|", $fb['after_info']);
        $arr_after_price = explode("|", $fb['after_price']);
        //print_r($arr_after_name);exit;

        foreach($arr_after_code as $key => $val) {

            $ret = array();
            $ret['mpcode'] = $val;
            $ret['after_name'] = $arr_after_name[$key];
            $ret['depth1'] = $arr_after_depth1[$key];
            $ret['depth2'] = $arr_after_depth2[$key];
            $ret['depth3'] = $arr_after_depth3[$key];
            $ret['detail'] = $arr_after_detail[$key];
            $ret['info'] = $arr_after_info[$key];
            $ret['price'] = $arr_after_price[$key];
            $ret['basic_yn'] = 'N';

            array_push($this->order_after_history_param, $ret);

        }

    }
}
