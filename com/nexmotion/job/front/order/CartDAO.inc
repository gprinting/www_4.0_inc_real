<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/OrderCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/order/CartHTML.inc');

class CartDAO extends OrderCommonDAO {
    function __construct() {
    }

    /**
     * @brief 카테고리 옵션 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["sell_site"] = 판매채널
     * $param["basic_yn"] = 기본여부
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리실행결과
     */
    function selectCateOptInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $except_arr = array("mpcode" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query .= "\n SELECT  A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  prdt_opt AS A";
        $query .= "\n        ,cate_opt AS B";

        $query .= "\n  WHERE  A.prdt_opt_seqno = B.prdt_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 실사 옵션 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["sell_site"] = 판매채널
     * $param["basic_yn"] = 기본여부
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리실행결과
     */
    function selectCateAoOptInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $except_arr = array("mpcode" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query .= "\n SELECT  A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  ao_opt AS A";
        $query .= "\n        ,cate_ao_opt AS B";

        $query .= "\n  WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 후공정 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["sell_site"] = 판매채널
     * $param["basic_yn"] = 기본여부
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리실행결과
     */
    function selectCateAfterInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param, ["mpcode" => true]);

        $query .= "\n SELECT  A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  prdt_after AS A";
        $query .= "\n        ,cate_after AS B";

        $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 실사 후공정 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["sell_site"] = 판매채널
     * $param["basic_yn"] = 기본여부
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리실행결과
     */
    function selectCateAoAfterInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $except_arr = ["mpcode" => true];

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query .= "\n SELECT  A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  ao_after AS A";
        $query .= "\n        ,cate_ao_after AS B";

        $query .= "\n  WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]);

        return $conn->Execute($query);
    }

    function insertOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO order_info_history ";
        $query .= "\n       (order_common_seqno, kind, before_detail, after_detail, empl_id) VALUES (%s, %s, %s, %s, %s)";

        $query  = sprintf($query, $param["order_common_seqno"], $param["kind"], $param["before"], $param["after"], $param["empl_id"]);
        return $conn->Execute($query);
    }


    function selectOrderDetailInfo($conn, $order_common_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query = "\n SELECT order_detail, amt, amt_unit_dvs, count FROM order_common where order_common_seqno = %s ";
        $query  = sprintf($query, $order_common_seqno);

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 주문_공통 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderCommon($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        //$except_arr = array("group_seqno" => true);
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);
        if($param["order_common_seqno"] == null) {
            $param["order_common_seqno"] = "NULL";
        }
        $query  = "\n INSERT INTO order_common (";
        $query .= "\n     order_regi_date";
        $query .= "\n     ,order_common_seqno";
        $query .= "\n     ,member_seqno";
        $query .= "\n     ,order_num";
        $query .= "\n     ,order_state";
        $query .= "\n     ,oper_sys";
        $query .= "\n     ,order_detail";
        $query .= "\n     ,mono_yn";
        $query .= "\n     ,claim_yn";
        $query .= "\n     ,title";
        $query .= "\n     ,sell_price";
        $query .= "\n     ,pay_price";
        $query .= "\n     ,grade_sale_price";
        $query .= "\n     ,member_sale_price";
        $query .= "\n     ,add_opt_price";
        $query .= "\n     ,add_after_price";
        $query .= "\n     ,event_price";
        $query .= "\n     ,expec_weight";
        $query .= "\n     ,cate_sortcode";
        $query .= "\n     ,file_upload_dvs";
        $query .= "\n     ,opt_use_yn";
        $query .= "\n     ,del_yn";
        $query .= "\n     ,group_seqno";
        $query .= "\n     ,amt";
        $query .= "\n     ,amt_unit_dvs";
        $query .= "\n     ,page_cnt";
        $query .= "\n     ,count";
        $query .= "\n     ,dlvr_produce_dvs";
        $query .= "\n     ,commerce_dvs";
        $query .= "\n     ,cust_memo";
        $query .= "\n     ,owncompany_img_num";
        $query .= "\n     ,receipt_dvs";
        $query .= "\n     ,bun_yn";
        $query .= "\n     ,dlvr_dvs";
        $query .= "\n ) VALUES (";
        $query .= "\n      now()";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,CONCAT(%s, LPAD((SELECT COUNT(*) + 1 FROM order_common AS B WHERE B.order_regi_date like '%s%%'), 5, '0'))";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n ) ON DUPLICATE KEY UPDATE ";
        $query .= "\n oper_sys = %s, ";
        $query .= "\n order_detail = %s, ";
        $query .= "\n mono_yn = %s, ";
        $query .= "\n claim_yn = %s, ";
        $query .= "\n title = %s, ";
        $query .= "\n sell_price = %s, ";
        $query .= "\n pay_price = %s, ";
        $query .= "\n grade_sale_price = %s, ";
        $query .= "\n member_sale_price = %s, ";
        $query .= "\n add_opt_price = %s, ";
        $query .= "\n add_after_price = %s, ";
        $query .= "\n event_price = %s, ";
        $query .= "\n expec_weight = %s, ";
        $query .= "\n cate_sortcode = %s, ";
        $query .= "\n file_upload_dvs = %s, ";
        $query .= "\n opt_use_yn = %s, ";
        $query .= "\n del_yn = %s, ";
        $query .= "\n group_seqno = %s, ";
        $query .= "\n amt = %s, ";
        $query .= "\n amt_unit_dvs = %s, ";
        $query .= "\n page_cnt = %s, ";
        $query .= "\n count = %s, ";
        $query .= "\n dlvr_produce_dvs = %s, ";
        $query .= "\n commerce_dvs = %s, ";
        $query .= "\n cust_memo = %s, ";
        $query .= "\n owncompany_img_num = %s, ";
        $query .= "\n receipt_dvs = %s, ";
        $query .= "\n bun_yn = %s, ";
        $query .= "\n dlvr_dvs = %s ";

        $query  = sprintf($query, $param["order_common_seqno"]
                                , $param["member_seqno"]
                                , $param["order_num"]
                                , date("Y-m-d")
                                , $param["order_state"]
                                , $param["oper_sys"]
                                , $param["order_detail"]
                                , $param["mono_yn"]
                                , $param["claim_yn"]
                                , $param["title"]
                                , $param["sell_price"]
                                , $param["pay_price"]
                                , $param["grade_sale_price"]
                                , $param["member_sale_price"]
                                , $param["add_opt_price"]
                                , $param["add_after_price"]
                                , $param["event_price"]
                                , $param["expec_weight"]
                                , $param["cate_sortcode"]
                                , $param["file_upload_dvs"]
                                , $param["opt_use_yn"]
                                , $param["del_yn"]
                                , $param["group_seqno"]
                                , $param["amt"]
                                , $param["amt_unit_dvs"]
                                , $param["page_cnt"]
                                , $param["count"]
                                , $param["dlvr_produce_dvs"]
                                , $param["commerce_dvs"]
                                , $param["cust_memo"]
                                , $param["owncompany_img_num"]
                                , $param["receipt_dvs"]
                                , $param["bun_yn"]
                                , $param["dlvr_dvs"]
                                , $param["oper_sys"]
                                , $param["order_detail"]
                                , $param["mono_yn"]
                                , $param["claim_yn"]
                                , $param["title"]
                                , $param["sell_price"]
                                , $param["pay_price"]
                                , $param["grade_sale_price"]
                                , $param["member_sale_price"]
                                , $param["add_opt_price"]
                                , $param["add_after_price"]
                                , $param["event_price"]
                                , $param["expec_weight"]
                                , $param["cate_sortcode"]
                                , $param["file_upload_dvs"]
                                , $param["opt_use_yn"]
                                , $param["del_yn"]
                                , $param["group_seqno"]
                                , $param["amt"]
                                , $param["amt_unit_dvs"]
                                , $param["page_cnt"]
                                , $param["count"]
                                , $param["dlvr_produce_dvs"]
                                , $param["commerce_dvs"]
                                , $param["cust_memo"]
                                , $param["owncompany_img_num"]
                                , $param["receipt_dvs"]
                                , $param["bun_yn"]
                                , $param["dlvr_dvs"]);
        //$conn->debug = 1;
        return $conn->Execute($query);
    }

    function insertOrderCommonBizhows($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //$except_arr = array("group_seqno" => true);
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);
        if($param["order_common_seqno"] == null) {
            $param["order_common_seqno"] = "NULL";
        }
        $query  = "\n INSERT INTO order_common (";
        $query .= "\n     order_regi_date";
        $query .= "\n     ,order_common_seqno";
        $query .= "\n     ,member_seqno";
        $query .= "\n     ,order_num";
        $query .= "\n     ,order_state";
        $query .= "\n     ,oper_sys";
        $query .= "\n     ,order_detail";
        $query .= "\n     ,mono_yn";
        $query .= "\n     ,claim_yn";
        $query .= "\n     ,title";
        $query .= "\n     ,sell_price";
        $query .= "\n     ,pay_price";
        $query .= "\n     ,grade_sale_price";
        $query .= "\n     ,member_sale_price";
        $query .= "\n     ,add_opt_price";
        $query .= "\n     ,add_after_price";
        $query .= "\n     ,event_price";
        $query .= "\n     ,expec_weight";
        $query .= "\n     ,cate_sortcode";
        $query .= "\n     ,file_upload_dvs";
        $query .= "\n     ,opt_use_yn";
        $query .= "\n     ,del_yn";
        $query .= "\n     ,group_seqno";
        $query .= "\n     ,amt";
        $query .= "\n     ,amt_unit_dvs";
        $query .= "\n     ,page_cnt";
        $query .= "\n     ,count";
        $query .= "\n     ,dlvr_produce_dvs";
        $query .= "\n     ,commerce_dvs";
        $query .= "\n     ,cust_memo";
        $query .= "\n     ,owncompany_img_num";
        $query .= "\n     ,receipt_dvs";
        $query .= "\n     ,bun_yn";
        $query .= "\n     ,dlvr_dvs";
        $query .= "\n     ,bun_group";//
        $query .= "\n     ,point_use_yn";//
        $query .= "\n     ,use_point_price";//
        $query .= "\n     ,depo_finish_date";//
        $query .= "\n     ,pay_way";//
        $query .= "\n ) VALUES (";
        $query .= "\n      now()";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s"; // LPAD((COUNT(*) + 1),5,'0')
        $query .= "\n     ,CONCAT(%s, LPAD((SELECT COUNT(*) + 1 FROM order_common AS B WHERE B.order_regi_date like '%s%%'), 5, '0'))";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";//
        $query .= "\n     ,%s";//
        $query .= "\n     ,%s";//
        $query .= "\n     ,%s";//
        $query .= "\n     ,%s";//
        $query .= "\n ) ON DUPLICATE KEY UPDATE ";
        $query .= "\n oper_sys = %s, ";
        $query .= "\n order_detail = %s, ";
        $query .= "\n mono_yn = %s, ";
        $query .= "\n claim_yn = %s, ";
        $query .= "\n title = %s, ";
        $query .= "\n sell_price = %s, ";
        $query .= "\n pay_price = %s, ";
        $query .= "\n grade_sale_price = %s, ";
        $query .= "\n member_sale_price = %s, ";
        $query .= "\n add_opt_price = %s, ";
        $query .= "\n add_after_price = %s, ";
        $query .= "\n event_price = %s, ";
        $query .= "\n expec_weight = %s, ";
        $query .= "\n cate_sortcode = %s, ";
        $query .= "\n file_upload_dvs = %s, ";
        $query .= "\n opt_use_yn = %s, ";
        $query .= "\n del_yn = %s, ";
        $query .= "\n group_seqno = %s, ";
        $query .= "\n amt = %s, ";
        $query .= "\n amt_unit_dvs = %s, ";
        $query .= "\n page_cnt = %s, ";
        $query .= "\n count = %s, ";
        $query .= "\n dlvr_produce_dvs = %s, ";
        $query .= "\n commerce_dvs = %s, ";
        $query .= "\n cust_memo = %s, ";
        $query .= "\n owncompany_img_num = %s, ";
        $query .= "\n receipt_dvs = %s, ";
        $query .= "\n bun_yn = %s, ";
        $query .= "\n dlvr_dvs = %s, ";
        $query .= "\n bun_group = %s, ";
        $query .= "\n point_use_yn = %s, ";
        $query .= "\n use_point_price = %s, ";
        $query .= "\n depo_finish_date = %s, ";
        $query .= "\n pay_way = %s ";

        $query  = sprintf($query, $param["order_common_seqno"]
            , $param["member_seqno"]
            , $param["order_num"]
            , date("Y-m-d")
            , $param["order_state"]
            , $param["oper_sys"]
            , $param["order_detail"]
            , $param["mono_yn"]
            , $param["claim_yn"]
            , $param["title"]
            , $param["sell_price"]
            , $param["pay_price"]
            , $param["grade_sale_price"]
            , $param["member_sale_price"]
            , $param["add_opt_price"]
            , $param["add_after_price"]
            , $param["event_price"]
            , $param["expec_weight"]
            , $param["cate_sortcode"]
            , $param["file_upload_dvs"]
            , $param["opt_use_yn"]
            , $param["del_yn"]
            , $param["group_seqno"]
            , $param["amt"]
            , $param["amt_unit_dvs"]
            , $param["page_cnt"]
            , $param["count"]
            , $param["dlvr_produce_dvs"]
            , $param["commerce_dvs"]
            , $param["cust_memo"]
            , $param["owncompany_img_num"]
            , $param["receipt_dvs"]
            , $param["bun_yn"]
            , $param["dlvr_dvs"]
            , $param["bun_group"]
            , $param["point_use_yn"]
            , $param["use_point_price"]
            , $param["depo_finish_date"]
            , $param["pay_way"]
            , $param["oper_sys"]
            , $param["order_detail"]
            , $param["mono_yn"]
            , $param["claim_yn"]
            , $param["title"]
            , $param["sell_price"]
            , $param["pay_price"]
            , $param["grade_sale_price"]
            , $param["member_sale_price"]
            , $param["add_opt_price"]
            , $param["add_after_price"]
            , $param["event_price"]
            , $param["expec_weight"]
            , $param["cate_sortcode"]
            , $param["file_upload_dvs"]
            , $param["opt_use_yn"]
            , $param["del_yn"]
            , $param["group_seqno"]
            , $param["amt"]
            , $param["amt_unit_dvs"]
            , $param["page_cnt"]
            , $param["count"]
            , $param["dlvr_produce_dvs"]
            , $param["commerce_dvs"]
            , $param["cust_memo"]
            , $param["owncompany_img_num"]
            , $param["receipt_dvs"]
            , $param["bun_yn"]
            , $param["dlvr_dvs"]
            , $param["bun_group"]
            , $param["point_use_yn"]
            , $param["use_point_price"]
            , $param["depo_finish_date"]
            , $param["pay_way"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_공통 테이블에 데이터 입력(재주문)
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderCommonForReOrder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        //$except_arr = array("group_seqno" => true);
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n INSERT INTO order_common (";
        $query .= "\n      order_regi_date";
        $query .= "\n     ,member_seqno";
        $query .= "\n     ,order_num";
        $query .= "\n     ,order_state";
        $query .= "\n     ,oper_sys";
        $query .= "\n     ,order_detail";
        $query .= "\n     ,mono_yn";
        $query .= "\n     ,claim_yn";
        $query .= "\n     ,title";
        $query .= "\n     ,sell_price";
        $query .= "\n     ,grade_sale_price";
        $query .= "\n     ,member_sale_price";
        $query .= "\n     ,add_opt_price";
        $query .= "\n     ,add_after_price";
        $query .= "\n     ,event_price";
        $query .= "\n     ,expec_weight";
        $query .= "\n     ,cate_sortcode";
        $query .= "\n     ,file_upload_dvs";
        $query .= "\n     ,opt_use_yn";
        $query .= "\n     ,del_yn";
        $query .= "\n     ,group_seqno";
        $query .= "\n     ,amt";
        $query .= "\n     ,amt_unit_dvs";
        $query .= "\n     ,page_cnt";
        $query .= "\n     ,count";
        $query .= "\n     ,dlvr_produce_dvs";
        $query .= "\n     ,commerce_dvs";
        $query .= "\n     ,cust_memo";
        $query .= "\n     ,point_use_yn";
        $query .= "\n     ,use_point_price";
        $query .= "\n     ,receipt_dvs";
        $query .= "\n     ,pay_way";
        $query .= "\n     ,bun_yn";
        $query .= "\n     ,bun_group";
        $query .= "\n ) VALUES (";
        $query .= "\n      now()";
        $query .= "\n     ,%s";
        $query .= "\n     ,CONCAT(%s, LPAD((SELECT COUNT(*) + 1 FROM order_common AS B WHERE B.order_regi_date like '%s%%'), 5, '0'))";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["order_num"]
            , date("Y-m-d")
                                , $param["order_state"]
                                , $param["oper_sys"]
                                , $param["order_detail"]
                                , $param["mono_yn"]
                                , $param["claim_yn"]
                                , $param["title"]
                                , $param["sell_price"]
                                , $param["grade_sale_price"]
                                , $param["member_sale_price"]
                                , $param["add_opt_price"]
                                , $param["add_after_price"]
                                , $param["event_price"]
                                , $param["expec_weight"]
                                , $param["cate_sortcode"]
                                , $param["file_upload_dvs"]
                                , $param["opt_use_yn"]
                                , $param["del_yn"]
                                , $param["group_seqno"]
                                , $param["amt"]
                                , $param["amt_unit_dvs"]
                                , $param["page_cnt"]
                                , $param["count"]
                                , $param["dlvr_produce_dvs"]
                                , $param["commerce_dvs"]
                                , $param["cust_memo"]
                                , $param["point_use_yn"]
                                , $param["use_point_price"]
                                , $param["receipt_dvs"]
                                , $param["pay_way"]
                                , $param["bun_yn"]
                                , $param["bun_group"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 정보 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderCommon($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  order_common";
        $query .= "\n    SET  order_state = %s";
        $query .= "\n        ,pay_price = %s";
        $query .= "\n        ,depo_price = %s";
        $query .= "\n        ,card_depo_price = %s";
        $query .= "\n        ,order_lack_price = %s";
        if ($this->blankParameterCheck($param, "depo_finish_date")) {
            $query .= "\n        ,depo_finish_date = " . $param["depo_finish_date"];
        }
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["pay_price"]
                                , $param["depo_price"]
                                , $param["card_depo_price"]
                                , $param["order_lack_price"]
                                , $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_상세 지불가격 정보 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  order_detail";
        $query .= "\n    SET  pay_price = %s";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param["pay_price"]
                                , $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_상세_브로셔 지불가격 정보 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  order_detail_brochure";
        $query .= "\n    SET  pay_price = %s";
        $query .= "\n  WHERE  order_detail_seqno = %s";

        $query  = sprintf($query, $param["pay_price"]
                                , $param["order_detail_seqno"]);

        return $conn->Execute($query);
    }

    function deleteOrderDetail($conn, $param) {
        $query  = "\n DELETE FROM order_detail";
        $query .= "\n    WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param);
        return $conn->Execute($query);
    }
    /**
     * @brief 주문_상세 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderDetail($conn, $param) {
        $order_detail_param[] = $detail_tmp;
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = [];
        $temp["table"] = "order_detail";
        $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];

        $temp["col"]["cate_beforeside_print_mpcode"] =
                                            $param["cate_bef_print_mpcode"];
        $temp["col"]["cate_aftside_print_mpcode"] =
                                            $param["cate_aft_print_mpcode"];
        $temp["col"]["cate_beforeside_add_print_mpcode"] =
                                            $param["cate_bef_add_print_mpcode"];
        $temp["col"]["cate_aftside_add_print_mpcode"] =
                                            $param["cate_aft_add_print_mpcode"];

        $temp["col"]["cate_paper_mpcode"]     = $param["cate_paper_mpcode"];
        $temp["col"]["cate_paper_tot_mpcode"] = $param["cate_paper_tot_mpcode"];
        $temp["col"]["cate_output_mpcode"]    = $param["cate_output_mpcode"];
        
        $temp["col"]["typ"] = $param["typ"];
        $temp["col"]["page_amt"] = $param["page_amt"];

        $temp["col"]["cut_size_wid"]  = $param["cut_size_wid"];
        $temp["col"]["cut_size_vert"] = $param["cut_size_vert"];
        $temp["col"]["work_size_wid"]  = $param["work_size_wid"];
        $temp["col"]["work_size_vert"] = $param["work_size_vert"];
        $temp["col"]["receipt_size_wid"]  = $param["work_size_wid"];
        $temp["col"]["receipt_size_vert"] = $param["work_size_vert"];
        $temp["col"]["tomson_size_wid"]  = $param["tomson_size_wid"];
        $temp["col"]["tomson_size_vert"] = $param["tomson_size_vert"];

        $temp["col"]["print_purp_dvs"] = $param["print_purp_dvs"];
        $temp["col"]["del_yn"] = 'N';

        $temp["col"]["spc_dscr"]             = $param["spc_dscr"];
        $temp["col"]["sell_price"]           = $param["sell_price"];
        $temp["col"]["grade_sale_price"]     = $param["grade_sale_price"];
        $temp["col"]["member_sale_price"]    = $param["member_sale_price"];
        $temp["col"]["add_after_price"]      = $param["add_after_price"];
        $temp["col"]["cp_price"]             = $param["cp_price"];
        $temp["col"]["pay_price"]            = $param["pay_price"];
        $temp["col"]["order_detail"]         = $param["order_detail"];
        $temp["col"]["mono_yn"]              = $param["mono_yn"];
        $temp["col"]["stan_name"]            = $param["stan_name"];
        $temp["col"]["amt"]                  = $param["amt"];
        $temp["col"]["count"]                = $param["count"];
        $temp["col"]["expec_weight"]         = $param["expec_weight"];
        $temp["col"]["amt_unit_dvs"]         = $param["amt_unit_dvs"];
        $temp["col"]["after_use_yn"]         = $param["after_use_yn"];
        $temp["col"]["cate_sortcode"]        = $param["cate_sortcode"];
        $temp["col"]["tot_tmpt"]             = $param["tot_tmpt"];
        $temp["col"]["side_dvs"]             = $param["side_dvs"];
        $temp["col"]["state"]                = $param["state"];
        $temp["col"]["order_detail_dvs_num"] = $param["order_detail_dvs_num"];
        $temp["col"]["print_tmpt_name"]      = $param["print_tmpt_name"];

        $temp["col"]["tomson_yn"] = $param["tomson_yn"];
        $temp["col"]["typset_way"] = $param["typset_way"];

        $temp["col"]["paper_price"]  = $param["paper_price"];
        $temp["col"]["output_price"] = $param["output_price"];
        $temp["col"]["print_price"]  = $param["print_price"];

        return $this->insertData($conn, $temp);
    }

    /**
     * @brief 주문_상세_책자 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = array();
        $temp["table"] = "order_detail_brochure";
        $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];

        $temp["col"]["cate_beforeside_print_mpcode"] =
                                            $param["cate_bef_print_mpcode"];
        $temp["col"]["cate_aftside_print_mpcode"] =
                                            $param["cate_aft_print_mpcode"];
        $temp["col"]["cate_beforeside_add_print_mpcode"] =
                                            $param["cate_bef_add_print_mpcode"];
        $temp["col"]["cate_aftside_add_print_mpcode"] =
                                            $param["cate_aft_add_print_mpcode"];

        $temp["col"]["cate_paper_mpcode"] = $param["cate_paper_mpcode"];

        $temp["col"]["cate_output_mpcode"] = $param["cate_output_mpcode"];
        
        $temp["col"]["typ"] = $param["typ"];
        $temp["col"]["page_amt"] = $param["page_amt"];

        $temp["col"]["cut_size_wid"]  = $param["cut_size_wid"];
        $temp["col"]["cut_size_vert"] = $param["cut_size_vert"];
        $temp["col"]["work_size_wid"]  = $param["work_size_wid"];
        $temp["col"]["work_size_vert"] = $param["work_size_vert"];
        $temp["col"]["tomson_size_wid"]  = $param["tomson_size_wid"];
        $temp["col"]["tomson_size_vert"] = $param["tomson_size_vert"];

        $temp["col"]["cut_front_wing_size_wid"] =
                                            $param["cut_front_wing_size_wid"];
        $temp["col"]["cut_front_wing_size_vert"] =
                                            $param["cut_front_wing_size_vert"];
        $temp["col"]["work_front_wing_size_wid"] =
                                            $param["work_front_wing_size_wid"];
        $temp["col"]["work_front_wing_size_vert"] =
                                            $param["work_front_wing_size_vert"];

        $temp["col"]["cut_rear_wing_size_wid"] =
                                             $param["cut_rear_wing_size_wid"];
        $temp["col"]["cut_rear_wing_size_vert"] =
                                             $param["cut_rear_wing_size_vert"];
        $temp["col"]["work_rear_wing_size_wid"] =
                                             $param["work_rear_wing_size_wid"];
        $temp["col"]["work_rear_wing_size_vert"] =
                                             $param["work_rear_wing_size_vert"];

        $temp["col"]["seneca_size"] = $param["seneca_size"];
        $temp["col"]["print_purp_dvs"] = $param["print_purp_dvs"];
        $temp["col"]["del_yn"] = 'N';

        $temp["col"]["spc_dscr"]             = $param["spc_dscr"];
        $temp["col"]["sell_price"]           = $param["sell_price"];
        $temp["col"]["grade_sale_price"]     = $param["grade_sale_price"];
        $temp["col"]["add_after_price"]      = $param["add_after_price"];
        $temp["col"]["cp_price"]             = $param["cp_price"];
        $temp["col"]["pay_price"]            = $param["pay_price"];
        $temp["col"]["order_detail"]         = $param["order_detail"];
        $temp["col"]["mono_yn"]              = $param["mono_yn"];
        $temp["col"]["stan_name"]            = $param["stan_name"];
        $temp["col"]["amt"]                  = $param["amt"];
        $temp["col"]["amt_unit_dvs"]         = $param["amt_unit_dvs"];
        $temp["col"]["expec_weight"]         = $param["expec_weight"];
        $temp["col"]["after_use_yn"]         = $param["after_use_yn"];
        $temp["col"]["cate_sortcode"]        = $param["cate_sortcode"];
        $temp["col"]["tot_tmpt"]             = $param["tot_tmpt"];
        $temp["col"]["side_dvs"]             = $param["side_dvs"];
        $temp["col"]["state"]                = $param["state"];
        $temp["col"]["order_detail_dvs_num"] = $param["order_detail_dvs_num"];
        $temp["col"]["print_tmpt_name"]      = $param["print_tmpt_name"];

        $temp["col"]["paper_price"]  = $param["paper_price"];
        $temp["col"]["output_price"] = $param["output_price"];
        $temp["col"]["print_price"]  = $param["print_price"];

        return $this->insertData($conn, $temp);
    }

    /**
     * @brief 주문_옵션_내역 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderOptHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = array();
        $temp["table"] = "order_opt_history";

        $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];

        $temp["col"]["mpcode"] = $param["mpcode"];

        $temp["col"]["opt_name"] = $param["opt_name"]   ;
        $temp["col"]["depth1"]   = $param["depth1"] ."x". $param["opt_cnt"];
        $temp["col"]["depth2"]   = $param["depth2"];
        $temp["col"]["depth3"]   = $param["depth3"];

        $temp["col"]["price"] = $param["price"];
        $temp["col"]["basic_yn"] = $param["basic_yn"];

        $temp["col"]["detail"] = $param["detail"];
        $temp["col"]["info"]   = $param["info"];

        return $this->insertData($conn, $temp);
    }

    /**
     * @brief 주문_후공정_내역 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertOrderAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = array();
        $temp["table"] = "order_after_history";

        $temp["col"]["order_detail_dvs_num"] = $param["order_detail_dvs_num"];
        $temp["col"]["order_common_seqno"]   = $param["order_common_seqno"];

        $temp["col"]["mpcode"] = $param["mpcode"];

        $temp["col"]["after_name"] = $param["after_name"];
        $temp["col"]["depth1"] = $param["depth1"];
        $temp["col"]["depth2"] = $param["depth2"];
        $temp["col"]["depth3"] = $param["depth3"];

        $temp["col"]["price"]    = $param["price"];
        $temp["col"]["basic_yn"] = $param["basic_yn"];
        $temp["col"]["seq"]      = $param["seq"];
        $temp["col"]["detail"]   = $param["detail"];
        $temp["col"]["info"]     = $param["info"];

        return $this->insertData($conn, $temp);
    }

    /**
     * @brief 각 에러단계에 따라서 주문관련 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteOrderData($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $except_arr = array("table" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "DELETE";
        $query .= "  FROM %s";
        $query .= " WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param["table"]
                                , $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_후공정_내역 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteOrderAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $order_detail_dvs_num = $this->arr2paramStr($conn,
                                                    $param["order_detail_dvs_num"]);

        $query  = "DELETE";
        $query .= "  FROM order_after_history";
        $query .= " WHERE order_detail_dvs_num IN (%s)";

        $query  = sprintf($query, $order_detail_dvs_num);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_후공정_내역 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteOrderAfterHistoryBySeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "DELETE";
        $query .= "  FROM order_after_history";
        $query .= " WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_후공정_내역 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteOrderOptHistoryBySeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "DELETE";
        $query .= "  FROM order_opt_history";
        $query .= " WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_상세_건수_파일 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteOrderDetailCountFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $order_detail_seqno = $this->arr2paramStr($conn,
                                                  $param["order_detail_seqno"]);

        $query  = "DELETE";
        $query .= "  FROM order_detail_count_file";
        $query .= " WHERE order_detail_seqno IN (%s)";

        $query  = sprintf($query, $order_detail_seqno);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문공통 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 삭제조건 파라미터
     *
     * @return option html
     */
    function deleteOrderCommon($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common";
        $query .= "\n SET order_state = '1080' ";
        $query .= "\n WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 인쇄 면구분, 총도수 검색
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 인쇄 맵핑코드
     *
     * @return option html
     */
    function selectPrintTmptInfo($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n SELECT  A.side_dvs";
        $query .= "\n        ,A.beforeside_tmpt";
        $query .= "\n        ,A.aftside_tmpt";
        $query .= "\n        ,A.add_tmpt";
        $query .= "\n        ,A.tot_tmpt";
        $query .= "\n   FROM  prdt_print AS A";
        $query .= "\n        ,cate_print AS B";
        $query .= "\n  WHERE  A.prdt_print_seqno = B.prdt_print_seqno";
        $query .= "\n    AND  B.mpcode IN (%s)";

        $query  = sprintf($query, $mpcode);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 판매채널 별명 검색
     *
     * @param $conn  = connection identifer
     * @param $seqno = 회사관리일련번호
     *
     * @return 검색결과
     */
    function selectCpnAdminNick($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n SELECT nick";
        $query .= "\n   FROM cpn_admin";
        $query .= "\n  WHERE cpn_admin_seqno = %s";

        $query  = sprintf($query, $seqno);

        $rs = $conn->Execute($query);

        return $rs->fields["nick"];
    }

    /**
     * @brief 주문_상세 테이블 검색
     *
     * @param $conn  = connection identifer
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetail($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $temp = [];
        $temp["col"]  = "\n  /* CartDAO.inc - 주문 상세 테이블 검색*/";
        $temp["col"] .= "\n  cate_beforeside_print_mpcode";
        $temp["col"] .= "\n ,cate_aftside_print_mpcode";
        $temp["col"] .= "\n ,cate_beforeside_add_print_mpcode";
        $temp["col"] .= "\n ,cate_aftside_add_print_mpcode";
        $temp["col"] .= "\n ,cate_paper_tot_mpcode";
        $temp["col"] .= "\n ,cate_paper_mpcode";
        $temp["col"] .= "\n ,cate_output_mpcode";
        $temp["col"] .= "\n ,typ";
        $temp["col"] .= "\n ,page_amt";
        $temp["col"] .= "\n ,cut_size_wid";
        $temp["col"] .= "\n ,cut_size_vert";
        $temp["col"] .= "\n ,work_size_wid";

        $temp["col"] .= "\n ,work_size_vert";
        $temp["col"] .= "\n ,tomson_size_wid";
        $temp["col"] .= "\n ,tomson_size_vert";
        $temp["col"] .= "\n ,print_purp_dvs";
        $temp["col"] .= "\n ,spc_dscr";
        $temp["col"] .= "\n ,order_detail";
        $temp["col"] .= "\n ,mono_yn";
        $temp["col"] .= "\n ,stan_name";
        $temp["col"] .= "\n ,amt";

        $temp["col"] .= "\n ,count";
        $temp["col"] .= "\n ,expec_weight";
        $temp["col"] .= "\n ,amt_unit_dvs";
        $temp["col"] .= "\n ,after_use_yn";
        $temp["col"] .= "\n ,cate_sortcode";
        $temp["col"] .= "\n ,tot_tmpt";
        $temp["col"] .= "\n ,side_dvs";
        $temp["col"] .= "\n ,order_detail_dvs_num";
        $temp["col"] .= "\n ,tomson_yn";
        $temp["col"] .= "\n ,typset_way";

        $temp["col"] .= "\n ,state";
        $temp["col"] .= "\n ,print_tmpt_name";

        $temp["table"] = "order_detail";

        $temp["where"]["order_common_seqno"] = $param["order_common_seqno"];

        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 카테고리 도수명 검색
     *
     * @param $conn  = connection identifer
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCatePrintName($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $query  = "\n SELECT  /* CartDAO.inc - 카테고리 도수명 검색 */";
        $query .= "\n         A.name";
        $query .= "\n   FROM  prdt_print AS A";
        $query .= "\n        ,cate_print AS B";
        $query .= "\n  WHERE  A.prdt_print_seqno = B.prdt_print_seqno";
        $query .= "\n    AND  B.mpcode = %s";

        $query  = sprintf($query, $conn->param($param["mpcode"]));

        $bind = [];
        $bind[] = $param["mpcode"];

        $rs = $conn->Execute($query, $bind);

        return $rs->fields["name"];
    }

    /**
     * @brief 주문_상세 테이블 검색
     *
     * @param $conn  = connection identifer
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailBrochure($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $temp = [];
        $temp["col"]  = "\n  /* CartDAO.inc - 주문 상세 책자 테이블 검색*/";
        $temp["col"] .= "\n  cate_sortcode";
        $temp["col"] .= "\n ,order_detail_dvs_num";
        $temp["col"] .= "\n ,state";
        $temp["col"] .= "\n ,typ";

        $temp["col"] .= "\n ,work_size_wid";
        $temp["col"] .= "\n ,work_size_vert";
        $temp["col"] .= "\n ,cut_size_wid";
        $temp["col"] .= "\n ,cut_size_vert";
        $temp["col"] .= "\n ,tomson_size_wid";
        $temp["col"] .= "\n ,tomson_size_vert";

        $temp["col"] .= "\n ,cut_front_wing_size_wid";
        $temp["col"] .= "\n ,cut_front_wing_size_vert";
        $temp["col"] .= "\n ,cut_rear_wing_size_wid";
        $temp["col"] .= "\n ,cut_rear_wing_size_vert";
        $temp["col"] .= "\n ,work_front_wing_size_wid";
        $temp["col"] .= "\n ,work_front_wing_size_vert";
        $temp["col"] .= "\n ,work_rear_wing_size_wid";
        $temp["col"] .= "\n ,work_rear_wing_size_vert";
        $temp["col"] .= "\n ,seneca_size";

        $temp["col"] .= "\n ,cate_paper_mpcode";
        $temp["col"] .= "\n ,cate_beforeside_print_mpcode";
        $temp["col"] .= "\n ,cate_aftside_print_mpcode";
        $temp["col"] .= "\n ,cate_beforeside_add_print_mpcode";
        $temp["col"] .= "\n ,cate_aftside_add_print_mpcode";
        $temp["col"] .= "\n ,cate_output_mpcode";

        $temp["col"] .= "\n ,order_detail";
        $temp["col"] .= "\n ,mono_yn";
        $temp["col"] .= "\n ,stan_name";
        $temp["col"] .= "\n ,print_tmpt_name";
        $temp["col"] .= "\n ,spc_dscr";
        $temp["col"] .= "\n ,print_purp_dvs";

        $temp["col"] .= "\n ,tot_tmpt";
        $temp["col"] .= "\n ,page_amt";

        $temp["col"] .= "\n ,amt";
        $temp["col"] .= "\n ,amt_unit_dvs";
        $temp["col"] .= "\n ,expec_weight";
        $temp["col"] .= "\n ,after_use_yn";
        $temp["col"] .= "\n ,side_dvs";

        $temp["table"] = "order_detail_brochure";

        $temp["where"]["order_common_seqno"] = $param["order_common_seqno"];

        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 주문_파일 정보 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE  order_file";
        $query .= "\n    SET  order_common_seqno = %s";
        $query .= "\n        ,save_file_name  = %s";
        $query .= "\n  WHERE  order_file_seqno = %s";
        $query .= "\n    AND  member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["order_common_seqno"])
                                , $conn->param($param["save_file_name"])
                                , $conn->param($param["order_file_seqno"])
                                , $conn->param($param["member_seqno"]));

        $temp = [];
        $temp[] = $param["order_common_seqno"];
        $temp[] = $param["save_file_name"];
        $temp[] = $param["order_file_seqno"];
        $temp[] = $param["member_seqno"];

        return $conn->Execute($query, $temp);
    }

    /**
     * @brief 주문_파일 정보 입력
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "order_file";
        $temp["col"]["dvs"]                = $param["dvs"];
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        $temp["col"]["member_seqno"]       = $param["member_seqno"];
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];
        }

        return $this->insertData($conn, $temp);
    }

}
?>
