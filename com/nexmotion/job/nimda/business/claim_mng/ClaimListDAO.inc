<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/business/claim_mng/ClaimListHTML.inc');

/**
 * @file MemberCommonListDAO.inc
 *
 * @brief 영업 - 클레임관리 - 클레임리스트 DAO
 */

class ClaimListDAO extends BusinessCommonDAO {
    function __construct() {
    }

    /**
     * @brief 클레임리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectClaimListByCond($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr  = array("state" => true,
                            "empl_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  =   "\n SELECT /* 클레임리스트 조건검색 쿼리 */ ";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .=   "\n        A.regi_date ";
        $query .=   "\n       ,C.member_name ";
        $query .=   "\n       ,C.office_nick ";
        $query .=   "\n       ,B.order_regi_date ";
        $query .=   "\n       ,B.order_num ";
        $query .=   "\n       ,B.title AS order_title";
        $query .=   "\n       ,A.title AS claim_title";
        $query .=   "\n       ,A.count ";
        $query .=   "\n       ,B.pay_price ";
        $query .=   "\n       ,A.dvs ";
        $query .=   "\n       ,A.state ";
        $query .=   "\n       ,A.agree_yn ";
        $query .=   "\n       ,A.order_claim_seqno ";
        $query .=   "\n       ,A.empl_seqno ";
        $query .=   "\n       ,B.member_seqno ";
        $query .=   "\n       ,B.cpn_admin_seqno ";
        $query .=   "\n  FROM  order_claim AS A ";
        $query .=   "\n       ,order_common AS B ";
        $query .=   "\n       ,member AS C ";
        $query .=   "\n WHERE  A.order_common_seqno = B.order_common_seqno ";
        $query .=   "\n   AND  B.member_seqno = C.member_seqno";
        if ($this->blankParameterCheck($param ,"claim_dvs")) {
            $query .= "\n   AND  A.dvs = ";
            $query .= $param["claim_dvs"];
        }
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.cpn_admin_seqno = ";
            $query .= $param["sell_site"];
        }
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  C.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param ,"member_typ")) {
            $query .= "\n   AND  C.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param ,"member_grade")) {
            $query .= "\n   AND  C.grade = ";
            $query .= $param["member_grade"];
        }
        // 팀단위 검색
        if ($this->blankParameterCheck($param ,"empl_seqno")) {
            $query .= "\n   AND  A.empl_seqno IN ( ";
            $query .= $param["empl_seqno"] . ")";
            // 해당 담당자만 검색
            if ($this->blankParameterCheck($param ,"empl")) {
                $query .= "\n   AND  A.empl_seqno = ";
                $query .= $param["empl"];
            }
        }
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  A.state LIKE ";
            $query .= "'%" . $param["state"] . "%' ";
        }
        if ($this->blankParameterCheck($param ,"from")) {
        //  $val = substr($param["search_cnd"], 1, -1);
            $query .= "\n   AND  A.regi_date >= $param[from] ";
        }
        if ($this->blankParameterCheck($param ,"to")) {
        //  $val = substr($param["search_cnd"], 1, -1); 기존쿼리는 A.$val 이었으나 별 의미가 없어보임
            $query .= "\n   AND  A.regi_date <= $param[to] ";
        }
        $query .= "\n  ORDER BY A.order_claim_seqno DESC ";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 클레임 상세보기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectClaimView($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
 
        if ($dvs == "COUNT") {
            $query  = "\n SELECT  COUNT(T1.order_claim_seqno) AS cnt";
        } else {
            $query  = "\n   SELECT T1.* ";
            $query .= "\n         ,T2.depar_code ";
            $query .= "\n         ,T2.name AS empl_name ";
        }
        $query .= "\n     FROM (";
        $query .= "\n               SELECT  A.regi_date ";
        $query .= "\n                      ,C.member_name ";
        $query .= "\n                      ,C.tel_num ";
        $query .= "\n                      ,C.cell_num ";
        $query .= "\n                      ,C.dlvr_code ";
        $query .= "\n                      ,C.addr ";
        $query .= "\n                      ,C.addr_detail ";
        $query .= "\n                      ,B.order_num ";
        $query .= "\n                      ,B.order_regi_date ";
        $query .= "\n                      ,B.order_detail ";
        $query .= "\n                      ,B.pay_price ";
        $query .= "\n                      ,B.title AS order_title";
        $query .= "\n                      ,B.receipt_mng ";
        $query .= "\n                      ,A.title ";
        $query .= "\n                      ,A.dvs ";
        $query .= "\n                      ,A.order_claim_seqno ";
        $query .= "\n                      ,A.sample_origin_file_name ";
        $query .= "\n                      ,A.empl_seqno ";
        $query .= "\n                      ,A.cust_cont ";
        $query .= "\n                      ,A.dvs_detail ";
        $query .= "\n                      ,A.mng_cont ";
        $query .= "\n                      ,A.agree_yn ";
        $query .= "\n                      ,A.order_yn ";
        $query .= "\n                      ,A.refund_prepay ";
        $query .= "\n                      ,A.refund_money ";
        $query .= "\n                      ,A.extnl_etprs_seqno ";
        $query .= "\n                      ,A.accident_cause ";
        $query .= "\n                      ,A.accident_type ";
        $query .= "\n                      ,A.occur_price ";
        $query .= "\n                      ,A.resp_dvs ";
        $query .= "\n                      ,A.outsource_burden_price ";
        $query .= "\n                      ,A.cust_burden_price ";
        $query .= "\n                      ,A.state ";
        $query .= "\n                      ,A.deal_date ";
        $query .= "\n                      ,A.count AS reorder_count ";
        $query .= "\n                      ,A.aprvl_code ";
        $query .= "\n                      ,A.aprvl_user ";
        $query .= "\n                      ,B.member_seqno ";
        $query .= "\n                      ,B.cpn_admin_seqno ";
        $query .= "\n                      ,B.order_common_seqno ";
        $query .= "\n                      ,B.count ";
        $query .= "\n                 FROM  order_claim AS A ";
        $query .= "\n                      ,order_common AS B ";
        $query .= "\n                      ,member AS C ";
        $query .= "\n                WHERE  A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n                  AND  B.member_seqno = C.member_seqno) AS T1 ";
        $query .= "\nLEFT JOIN  empl AS T2 ";
        $query .= "\n       ON  T1.empl_seqno = T2.empl_seqno ";
        if ($this->blankParameterCheck($param ,"order_claim_seqno")) {
            $query .= "\n  WHERE  T1.order_claim_seqno = ";
            $query .= $param["order_claim_seqno"];
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 일련번호로 주문 내용 팝업 html 생성
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     *
     * @return 주문정보팝업 html
     */
    function selectOrderInfoNonePop($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query  = "\n SELECT  prdt_basic_info";
        $query .= "\n        ,prdt_add_info";
        $query .= "\n        ,prdt_price_info";
        $query .= "\n        ,prdt_pay_info";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $seqno);
        $rs = $conn->Execute($query);

        return makeOrderInfoNonePopHtml($rs->fields);
    }

    /**
     * @brief 주문 일련번호로 주문 내용 팝업 html 생성
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     *
     * @return 주문정보팝업 html
     */
    function selectFlattypYn($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query  = "\n SELECT  B.flattyp_yn";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n        ,cate AS B";
        $query .= "\n  WHERE  A.cate_sortcode = B.sortcode";
        $query .= "\n    AND  A.order_common_seqno = %s";

        $query  = sprintf($query, $seqno);
        return $conn->Execute($query);
    }


    /**
     * @brief 주문 공통 재주문을 위한 정보검색
     * @param $conn  = connection identifier
     */
    function selectReOrder($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query  = "\n SELECT";
        $query .= "\n         member_seqno";
        $query .= "\n        ,cpn_admin_seqno";
        $query .= "\n        ,order_mng";
        $query .= "\n        ,title";
        $query .= "\n        ,group_seqno";
        $query .= "\n        ,oper_sys";
        $query .= "\n        ,order_detail";
        $query .= "\n        ,bun_group";
        $query .= "\n        ,mono_yn";
        $query .= "\n        ,claim_yn";
        $query .= "\n        ,point_use_yn";
        $query .= "\n        ,event_yn";
        $query .= "\n        ,order_regi_date";
        $query .= "\n        ,depo_finish_date";
        $query .= "\n        ,expec_weight";
        $query .= "\n        ,cust_memo";
        $query .= "\n        ,del_yn";
        $query .= "\n        ,eraser";
        $query .= "\n        ,owncompany_img_num";
        $query .= "\n        ,pay_way";
        $query .= "\n        ,cate_sortcode";
        $query .= "\n        ,opt_use_yn";
        $query .= "\n        ,prdt_basic_info";
        $query .= "\n        ,prdt_add_info";
        $query .= "\n        ,prdt_price_info";
        $query .= "\n        ,prdt_pay_info";
        $query .= "\n        ,receipt_dvs";
        $query .= "\n        ,bun_yn";
        $query .= "\n        ,file_upload_dvs";
        $query .= "\n        ,amt";
        $query .= "\n        ,amt_unit_dvs";
        $query .= "\n        ,page_cnt";
        $query .= "\n        ,count";
        $query .= "\n        ,dlvr_produce_dvs";
        $query .= "\n        ,qc_check_pc";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  order_common_seqno = ";
        $query .= $seqno;

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 주문 공통 재주문
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     *
     */
    function insertReOrder($conn, $result, $param) {
        
        $param = $this->parameterArrayEscape($conn, $param);

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\nINSERT INTO order_common (";
        $query .= "\n       order_num";
        $query .= "\n      ,order_state";
        $query .= "\n      ,member_seqno";
        $query .= "\n      ,cpn_admin_seqno";
        $query .= "\n      ,order_mng";
        $query .= "\n      ,title";
        $query .= "\n      ,group_seqno";
        $query .= "\n      ,oper_sys";
        $query .= "\n      ,order_detail";
        $query .= "\n      ,bun_group";
        $query .= "\n      ,mono_yn";
        $query .= "\n      ,claim_yn";
        $query .= "\n      ,point_use_yn";
        $query .= "\n      ,event_yn";
        $query .= "\n      ,use_point_price";
        $query .= "\n      ,grade_sale_price";
        $query .= "\n      ,member_sale_price";
        $query .= "\n      ,add_after_price";
        $query .= "\n      ,add_opt_price";
        $query .= "\n      ,event_price";
        $query .= "\n      ,cp_price";
        $query .= "\n      ,sell_price";
        $query .= "\n      ,pay_price";
        $query .= "\n      ,depo_price";
        $query .= "\n      ,order_lack_price";
        $query .= "\n      ,order_regi_date";
        $query .= "\n      ,depo_finish_date";
        $query .= "\n      ,expec_weight";
        $query .= "\n      ,cust_memo";
        $query .= "\n      ,del_yn";
        $query .= "\n      ,eraser";
        $query .= "\n      ,owncompany_img_num";
        $query .= "\n      ,pay_way";
        $query .= "\n      ,cate_sortcode";
        $query .= "\n      ,opt_use_yn";
        $query .= "\n      ,prdt_basic_info";
        $query .= "\n      ,prdt_add_info";
        $query .= "\n      ,prdt_price_info";
        $query .= "\n      ,prdt_pay_info";
        $query .= "\n      ,receipt_dvs";
        $query .= "\n      ,bun_yn";
        $query .= "\n      ,file_upload_dvs";
        $query .= "\n      ,amt";
        $query .= "\n      ,amt_unit_dvs";
        $query .= "\n      ,page_cnt";
        $query .= "\n      ,count";
        $query .= "\n      ,dlvr_produce_dvs";
        $query .= "\n      ,qc_check_pc";
        $query .= "\n      ) VALUES (";
        $query .= "\n        " . $param["order_num"] . " ";
        $query .= "\n      , " . $param["order_state"] . " ";
        $query .= "\n      ,'" . $result->fields["member_seqno"] . "'";
        $query .= "\n      ,'" . $result->fields["cpn_admin_seqno"] . "'";
        $query .= "\n      ,'" . $result->fields["order_mng"] . "'";
        $query .= "\n      ,'" . $result->fields["title"] . "'";
        $query .= "\n      ,'" . $result->fields["group_seqno"] . "'";
        $query .= "\n      ,'" . $result->fields["oper_sys"] . "'";
        $query .= "\n      ,'" . $result->fields["order_detail"] . "'";
        $query .= "\n      ,'" . $result->fields["bun_group"] . "'";
        $query .= "\n      ,'" . $result->fields["mono_yn"] . "'";
        $query .= "\n      , " . $param["claim_yn"] . " ";
        $query .= "\n      ,'" . $result->fields["point_use_yn"] . "'";
        $query .= "\n      ,'" . $result->fields["event_yn"] . "'";
        $query .= "\n      , " . $param["use_point_price"] . " ";
        $query .= "\n      , " . $param["grade_sale_price"] . " ";
        $query .= "\n      , " . $param["member_sale_price"] . " ";
        $query .= "\n      , " . $param["add_after_price"] . " ";
        $query .= "\n      , " . $param["add_opt_price"] . " ";
        $query .= "\n      , " . $param["event_price"] . " ";
        $query .= "\n      , " . $param["cp_price"] . " ";
        $query .= "\n      , " . $param["sell_price"] . " ";
        $query .= "\n      , " . $param["pay_price"] . " ";
        $query .= "\n      , " . $param["depo_price"] . " ";
        $query .= "\n      , " . $param["order_lack_price"] . " ";
        $query .= "\n      ,'" . $result->fields["order_regi_date"] . "'";
        $query .= "\n      ,now()";
        $query .= "\n      ,'" . $result->fields["expec_weight"] . "'";
        $query .= "\n      ,'" . $result->fields["cust_memo"] . "'";
        $query .= "\n      ,'" . $result->fields["del_yn"] . "'";
        $query .= "\n      ,'" . $result->fields["eraser"] . "'";
        $query .= "\n      ,'" . $result->fields["owncompany_img_num"] . "'";
        $query .= "\n      ,'" . $result->fields["pay_way"] . "'";
        $query .= "\n      ,'" . $result->fields["cate_sortcode"] . "'";
        $query .= "\n      ,'" . $result->fields["opt_use_yn"] . "'";
        $query .= "\n      ,'" . $result->fields["prdt_basic_info"] . "'";
        $query .= "\n      ,'" . $result->fields["prdt_add_info"] . "'";
        $query .= "\n      ,'" . $result->fields["prdt_price_info"] . "'";
        $query .= "\n      ,'" . $result->fields["prdt_pay_info"] . "'";
        $query .= "\n      , " . $param["receipt_dvs"] . " ";
        $query .= "\n      ,'" . $result->fields["bun_yn"] . "'";
        $query .= "\n      ,'" . $result->fields["file_upload_dvs"] . "'";
        $query .= "\n      ,'" . $result->fields["amt"] . "'";
        $query .= "\n      ,'" . $result->fields["amt_unit_dvs"] . "'";
        $query .= "\n      ,'" . $result->fields["page_cnt"] . "'";
        $query .= "\n      ,'" . $result->fields["count"] . "'";
        $query .= "\n      ,'" . $result->fields["dlvr_produce_dvs"] . "'";
        $query .= "\n      ,'" . $result->fields["qc_check_pc"] . "'";
        $query .= "\n )";

        $resultSet = $conn->Execute($query);

        if ($resultSet === FALSE) {
            return false;
        } else { 
            return true;
        }
    }

    /**
     * @brief 주문 상세 재주문 - 책자형
     *
     * @param $conn  = connection identifier
     * @param $param = 주문 파라미터
     * @comment 17.08.11부로 주석처리
     *
    function selectReOrderDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\nINSERT INTO order_detail_brochure";
        $query .= "\n      (order_common_seqno";
        $query .= "\n      ,cate_sortcode";
        $query .= "\n      ,order_detail_dvs_num";
        $query .= "\n      ,state";
        $query .= "\n      ,typ";
        $query .= "\n      ,paper_price";
        $query .= "\n      ,output_price";
        $query .= "\n      ,print_price";
        $query .= "\n      ,paper_sum_price";
        $query .= "\n      ,output_sum_price";
        $query .= "\n      ,print_sum_price";
        $query .= "\n      ,sell_price";
        $query .= "\n      ,grade_sale_price";
        $query .= "\n      ,member_sale_price";
        $query .= "\n      ,use_point_price";
        $query .= "\n      ,add_after_price";
        $query .= "\n      ,cp_price";
        $query .= "\n      ,pay_price";
        $query .= "\n      ,del_yn";
        $query .= "\n      ,work_size_wid";
        $query .= "\n      ,work_size_vert";
        $query .= "\n      ,cut_size_wid";
        $query .= "\n      ,cut_size_vert";
        $query .= "\n      ,tomson_size_wid";
        $query .= "\n      ,tomson_size_vert";
        $query .= "\n      ,cut_front_wing_size_wid";
        $query .= "\n      ,cut_front_wing_size_vert";
        $query .= "\n      ,cut_rear_wing_size_wid";
        $query .= "\n      ,cut_rear_wing_size_vert";
        $query .= "\n      ,work_front_wing_size_wid";
        $query .= "\n      ,work_front_wing_size_vert";
        $query .= "\n      ,work_rear_wing_size_wid";
        $query .= "\n      ,work_rear_wing_size_vert";
        $query .= "\n      ,seneca_size";
        $query .= "\n      ,cate_paper_mpcode";
        $query .= "\n      ,cate_beforeside_print_mpcode";
        $query .= "\n      ,cate_beforeside_add_print_mpcode";
        $query .= "\n      ,cate_aftside_print_mpcode";
        $query .= "\n      ,cate_aftside_add_print_mpcode";
        $query .= "\n      ,cate_output_mpcode";
        $query .= "\n      ,order_detail";
        $query .= "\n      ,mono_yn";
        $query .= "\n      ,stan_name";
        $query .= "\n      ,print_tmpt_name )";
        $query .= "\nSELECT " . $param["order_common_seqno"] . " AS order_common_seqno ";
        $query .= "\n      ,A.cate_sortcode";
        $query .= "\n      ,A.order_detail_dvs_num";
        $query .= "\n      ," . $param["state"] . " AS state ";
        $query .= "\n      ,A.typ";
        $query .= "\n      ," . $param["paper_price"] . " AS paper_price";
        $query .= "\n      ," . $param["output_price"] . " AS output_price";
        $query .= "\n      ," . $param["print_price"] . " AS print_price";
        $query .= "\n      ," . $param["paper_sum_price"] . " AS paper_sum_price";
        $query .= "\n      ," . $param["output_sum_price"] . " AS output_sum_price";
        $query .= "\n      ," . $param["print_sum_price"] . " AS print_sum_price";
        $query .= "\n      ," . $param["sell_price"] . " AS sell_price";
        $query .= "\n      ," . $param["grade_sale_price"] . " AS grade_sale_price";
        $query .= "\n      ," . $param["member_sale_price"] . " AS member_sale_price";
        $query .= "\n      ," . $param["use_point_price"] . " AS use_point_price";
        $query .= "\n      ," . $param["add_after_price"] . " AS add_after_price";
        $query .= "\n      ," . $param["cp_price"] . " AS cp_price";
        $query .= "\n      ," . $param["pay_price"] . " AS pay_price";
        $query .= "\n      ," . $param["del_yn"] . " AS del_yn";
        $query .= "\n      ,A.work_size_wid";
        $query .= "\n      ,A.work_size_vert";
        $query .= "\n      ,A.cut_size_wid";
        $query .= "\n      ,A.cut_size_vert";
        $query .= "\n      ,A.tomson_size_wid";
        $query .= "\n      ,A.tomson_size_vert";
        $query .= "\n      ,A.cut_front_wing_size_wid";
        $query .= "\n      ,A.cut_front_wing_size_vert";
        $query .= "\n      ,A.cut_rear_wing_size_wid";
        $query .= "\n      ,A.cut_rear_wing_size_vert";
        $query .= "\n      ,A.work_front_wing_size_wid";
        $query .= "\n      ,A.work_front_wing_size_vert";
        $query .= "\n      ,A.work_rear_wing_size_wid";
        $query .= "\n      ,A.work_rear_wing_size_vert";
        $query .= "\n      ,A.seneca_size";
        $query .= "\n      ,A.cate_paper_mpcode";
        $query .= "\n      ,A.cate_beforeside_print_mpcode";
        $query .= "\n      ,A.cate_beforeside_add_print_mpcode";
        $query .= "\n      ,A.cate_aftside_print_mpcode";
        $query .= "\n      ,A.cate_aftside_add_print_mpcode";
        $query .= "\n      ,A.cate_output_mpcode";
        $query .= "\n      ,A.order_detail";
        $query .= "\n      ,A.mono_yn";
        $query .= "\n      ,A.stan_name";
        $query .= "\n      ,A.print_tmpt_name ";
        $query .= "\n FROM  order_detail_brochure A";
        $query .= "\nWHERE order_common_seqno=" . $param["order_common_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === FALSE) {
            return false;
        } else { 
            return true;
        }
    } */

    /**
     * @brief order_detail 테이블 조회
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     */
    function selectOrderDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.cate_sortcode";
        $query  = "\n        ,A.order_detail_dvs_num";
        $query  = "\n        ,A.typ";
        $query  = "\n        ,A.work_size_wid";
        $query  = "\n        ,A.work_size_vert";
        $query  = "\n        ,A.cut_size_wid";
        $query  = "\n        ,A.cut_size_vert";
        $query  = "\n        ,A.tomson_size_wid";
        $query  = "\n        ,A.tomson_size_vert";
        $query  = "\n        ,A.cut_front_wing_size_wid";
        $query  = "\n        ,A.cut_front_wing_size_vert";
        $query  = "\n        ,A.cut_rear_wing_size_wid";
        $query  = "\n        ,A.cut_rear_wing_size_vert";
        $query  = "\n        ,A.work_front_wing_size_wid";
        $query  = "\n        ,A.work_front_wing_size_vert";
        $query  = "\n        ,A.seneca_size";
        $query  = "\n        ,A.cate_paper_mpcode";
        $query  = "\n        ,A.cate_beforeside_print_mpcode";
        $query  = "\n        ,A.cate_beforeside_add_print_mpcode";
        $query  = "\n        ,A.cate_aftside_print_mpcode";
        $query  = "\n        ,A.cate_aftside_add_print_mpcode";
        $query  = "\n        ,A.cate_output_mpcode";
        $query  = "\n        ,A.order_detail";
        $query  = "\n        ,A.mono_yn";
        $query  = "\n        ,A.stan_name";
        $query  = "\n        ,A.print_tmpt_name";
        $query  = "\n        ,A.spc_dscr";
        $query  = "\n        ,A.print_purp_dvs";
        $query  = "\n        ,A.tot_tmpt";
        $query  = "\n        ,A.page_amt";
        $query  = "\n        ,A.amt";
        $query  = "\n        ,A.amt_unit_dvs";
        $query  = "\n        ,A.expec_weight";
        $query  = "\n        ,A.after_use_yn";
        $query  = "\n        ,A.prdt_basic_info";
        $query  = "\n        ,A.prdt_add_info";
        $query  = "\n        ,A.side_dvs";
        $query .= "\n   FROM  order_detail_brochure AS A";
        $query .= "\n  WHERE  order_common_seqno = %s";
        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }

    /**
     * @brief member_seqno 조회
     *        order_file 테이블에 INSERT 하기위함.
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     */
    function selectMemberSeqno($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $seqno = $this->parameterEscape($conn, $seqno);
 
        $query  = "\n SELECT  member_seqno";
        $query .= "\n        ,order_num";
        $query .= "\n        ,opt_use_yn";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $seqno);
        return $conn->Execute($query);
    }

    /**
     * @brief empl_seqno 조회
     *        팀단위 검색을 하기 위함.
     * @param $conn  = connection identifier
     */
    function selectEmplTeam($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.empl_seqno";
        $query .= "\n   FROM  empl AS A";
        $query .= "\n  WHERE  A.depar_code = %s";

        $query  = sprintf($query, $param["depar"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief team deparcode 조회
     *        회원정보 팀구분코드값 구하기 위함
     * @param $conn  = connection identifier
     */
    function selectMemberTeamCode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.resp_deparcode";
        $query .= "\n   FROM  member_mng AS A";
        $query .= "\n        ,member AS B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  B.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 관할 team name 조회
     *        회원정보 팀구분 코드값을 이름으로 변환
     * @param $conn  = connection identifier
     */
    function selectMemberTeamName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.depar_name";
        $query .= "\n   FROM  depar_admin AS A";
        $query .= "\n  WHERE  A.depar_code = %s";

        $query  = sprintf($query, $param["resp_deparcode"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }
    
     /**
     * @brief 클레임 상품명 조회
     * @param $conn  = connection identifier
     */
    function selectClaimPrdtName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  B.cate_name";
        $query .= "\n        ,B.sortcode";
        $query .= "\n        ,B.flattyp_yn";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n        ,cate AS B";
        $query .= "\n  WHERE  A.cate_sortcode = B.sortcode";
        $query .= "\n    AND  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역 규격, 수량, 건수  조회
     *        flattype_yn에 따라 구별
     * @param $conn  = connection identifier
     */
    function selectClaimOrderDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.cate_paper_mpcode";
        $query .= "\n        ,A.stan_name";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.count";
        $query .= "\n   FROM  order_detail AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역 규격, 수량, 건수  조회
     *        flattype_yn에 따라 구별
     * @param $conn  = connection identifier
     */
    function selectClaimOrderDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.cate_paper_mpcode";
        $query .= "\n        ,A.stan_name";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.count AS 1";
        $query .= "\n   FROM  order_detail_brochure AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역 재질 조회
     * @param $conn  = connection identifier
     */
    function selectClaimOrderPaper($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.name";
        $query .= "\n   FROM  cate_paper AS A";
        $query .= "\n  WHERE  A.mpcode = %s";

        $query  = sprintf($query, $param["mpcode"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역 도수 조회
     * @param $conn  = connection identifier
     */
    function selectClaimOrderDegree($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  B.name";
        $query .= "\n   FROM  cate_print AS A";
        $query .= "\n        ,prdt_print AS B";
        $query .= "\n  WHERE  A.prdt_print_seqno = B.prdt_print_seqno";
        $query .= "\n    AND  A.cate_sortcode = %s";
        $query .= "\n    AND  A.mpcode = %s";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["mpcode"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역 후가공(후공정) 조회
     * @param $conn  = connection identifier
     */
    function selectClaimOrderAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.after_name";
        $query .= "\n   FROM  order_after_history AS A";
        $query .= "\n  WHERE  A.basic_yn = \"N\"";
        $query .= "\n    AND  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역, 클레임 합의정보 판번호 조회(1)
     * @comment 판번호를 한번에 불러올 수 없으므로 쿼리를 나눔
     * @param $conn  = connection identifier
     */
    function selectSheetTypsetSeq($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.sheet_typset_seqno";
        $query .= "\n   FROM  amt_order_detail_sheet AS A";
        $query .= "\n  WHERE  A.order_num = %s";

        $query  = sprintf($query, $param["order_num"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 회원주문내역, 클레임 합의정보 판번호 조회(2)
     * @comment 판번호를 한번에 불러올 수 없으므로 쿼리를 나눔
     * @param $conn  = connection identifier
     */
    function selectTypsetNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.typset_num";
        $query .= "\n   FROM  sheet_typset AS A";
        $query .= "\n  WHERE  A.sheet_typset_seqno = %s";

        $query  = sprintf($query, $param["sheet_typset_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 클레임 승인 업데이트
     * @param $conn  = connection identifier
     */
    function updateAprvlData($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  order_claim";
        $query .= "\n    SET  aprvl_code = %s";
        $query .= "\n        ,aprvl_user = %s";
        $query .= "\n  WHERE  order_claim_seqno = %s";

        $query  = sprintf($query, $param["aprvl_code"]
                                , $param["aprvl_user"]
                                , $param["claim_seqno"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

   
}
?>
