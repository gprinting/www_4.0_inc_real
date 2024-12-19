<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/MypageCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/mypage/estimate_list/EstiListHTML.inc');

/**
 * @file EstiInfoDAO.inc
 *
 * @brief 마이페이지 - 견적관리 - 견적리스트 DAO
 */

class EstiInfoDAO extends MypageCommonDAO {
    
    function __construct() {
    }
 
    /**
     * @brief 견적리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiListCond($conn, $param) {
     
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiListCond */";
        $query .= "\n         A.esti_seqno ";
        $query .= "\n        ,A.regi_date ";
        $query .= "\n        ,A.title ";
        $query .= "\n        ,A.esti_detail";
        $query .= "\n        ,A.order_price ";
        $query .= "\n        ,A.esti_date ";
        $query .= "\n        ,A.esti_mng ";
        $query .= "\n        ,A.state ";
        $query .= "\n        ,A.final_order_date ";
        $query .= "\n   FROM  esti AS A ";
        $query .= "\n  WHERE  1 = 1";

        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n    AND  A.member_seqno     = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param ,"group_seqno")) {
            $query .= "\n    AND  A.group_seqno     = ";
            $query .= $param["group_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n    AND  A.state     = ";
            $query .= $param["state"];
        }
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $from = substr($param["from"], 1, -1);
            $query .= "\n    AND  A.$val > '" . $from;
            $query .=" 00:00:00'";
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $to = substr($param["to"], 1, -1);
            $query .= "\n    AND  A.$val <= '" . $to;
            $query .=" 23:59:59'";
        }
        if ($this->blankParameterCheck($param ,"title")) {
            $title = substr($param["title"], 1, -1);
            $query .= "\n    AND  A.title LIKE '%" . $title . "%'";
        }
        
        $query .= "\n ORDER BY esti_seqno DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if (!$dvs) { 
            $query .= "\n LIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 견적정보 중 기본정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiBaseInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiBaseInfo, 견적정보 중 기본정보 */";
        $query .= "\n         A.title";
        $query .= "\n        ,A.esti_detail";
        $query .= "\n        ,A.memo";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.amt_note";
        $query .= "\n        ,A.page_cnt";
        $query .= "\n        ,A.expec_weight";
        $query .= "\n        ,A.origin_price";
        $query .= "\n        ,A.sale_rate";
        $query .= "\n        ,A.sale_price";
        $query .= "\n        ,A.esti_price";
        $query .= "\n        ,A.vat";
        $query .= "\n        ,A.order_price";
        $query .= "\n        ,A.state";
        $query .= "\n        ,A.esti_memo";
        $query .= "\n   FROM  esti AS A";
        $query .= "\n  WHERE  A.esti_seqno   = %s";
        $query .= "\n    AND  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["esti_seqno"])
                                , $conn->param($param["member_seqno"]));

        $temp = [
             $param["esti_seqno"]
            ,$param["member_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }

    /**
     * @brief 견적정보 낱장 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiInfoDetailSheet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiInfoDetailSheet, 견적정보 낱장 */";
        $query .= "\n         A.esti_detail_dvs_num";
        $query .= "\n        ,A.expec_weight";
        $query .= "\n        ,A.paper_info";
        $query .= "\n        ,A.paper_info_note";
        $query .= "\n        ,A.size_info";
        $query .= "\n        ,A.work_size_wid";
        $query .= "\n        ,A.work_size_vert";
        $query .= "\n        ,A.cut_size_wid";
        $query .= "\n        ,A.cut_size_vert";
        $query .= "\n        ,A.tomson_size_wid";
        $query .= "\n        ,A.tomson_size_vert";
        $query .= "\n        ,A.beforeside_tmpt_info      AS bef_tmpt_info";
        $query .= "\n        ,A.beforeside_tmpt_info_note AS bef_tmpt_info_note";
        $query .= "\n        ,A.aftside_tmpt_info         AS aft_tmpt_info";
        $query .= "\n        ,A.aftside_tmpt_info_note    AS aft_tmpt_info_note";
        $query .= "\n        ,A.print_purp_info";
        $query .= "\n        ,A.print_purp_info_note";
        $query .= "\n        ,A.after_info";
        $query .= "\n        ,A.after_info_note";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.page_amt";
        $query .= "\n        ,A.paper_mpcode";
        $query .= "\n        ,A.beforeside_print_mpcode     AS bef_print_mpcode";
        $query .= "\n        ,A.beforeside_add_print_mpcode AS bef_add_print_mpcode";
        $query .= "\n        ,A.aftside_print_mpcode        AS aft_print_mpcode";
        $query .= "\n        ,A.aftside_add_print_mpcode    AS aft_add_print_mpcode";
        $query .= "\n        ,A.output_mpcode";
        //$query .= "\n        ,A.esti_detail_dvs_num";
        //$query .= "\n        ,A.paper_unitprice";
        //$query .= "\n        ,A.paper_machine_amt";
        //$query .= "\n        ,A.paper_R_amt";
        $query .= "\n        ,A.paper_price";
        //$query .= "\n        ,A.output";
        //$query .= "\n        ,A.output_unitprice";
        //$query .= "\n        ,A.output_machine_amt";
        //$query .= "\n        ,A.output_R_amt";
        $query .= "\n        ,A.output_price";
        //$query .= "\n        ,A.print";
        $query .= "\n        ,A.print_tmpt";
        $query .= "\n        ,A.tot_tmpt";
        //$query .= "\n        ,A.print_unitprice";
        //$query .= "\n        ,A.print_machine_amt";
        //$query .= "\n        ,A.print_R_amt";
        $query .= "\n        ,A.print_price";
        $query .= "\n   FROM  esti_detail AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $conn->param($param["esti_seqno"]));

        $rs = $conn->Execute($query, [$param["esti_seqno"]]);

        return $rs;
    }
  
    /**
     * @brief 견적정보 책자 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiInfoDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiInfoDetailBrochure, 견적정보 책자 */";
        $query .= "\n         A.esti_detail_dvs_num";
        $query .= "\n        ,A.expec_weight";
        $query .= "\n        ,A.paper_info";
        $query .= "\n        ,A.paper_info_note";
        $query .= "\n        ,A.size_info";
        $query .= "\n        ,A.work_size_wid";
        $query .= "\n        ,A.work_size_vert";
        $query .= "\n        ,A.cut_size_wid";
        $query .= "\n        ,A.cut_size_vert";
        $query .= "\n        ,A.beforeside_tmpt_info AS bef_tmpt_info";
        $query .= "\n        ,A.beforeside_tmpt_info_note AS bef_tmpt_info_note";
        $query .= "\n        ,A.aftside_tmpt_info AS aft_tmpt_info";
        $query .= "\n        ,A.aftside_tmpt_info_note AS aft_tmpt_info_note";
        $query .= "\n        ,A.print_purp_info";
        $query .= "\n        ,A.print_purp_info_note";
        $query .= "\n        ,A.after_info";
        $query .= "\n        ,A.after_info_note";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.page_amt";
        $query .= "\n        ,A.paper_mpcode";
        $query .= "\n        ,A.beforeside_print_mpcode     AS bef_print_mpcode";
        $query .= "\n        ,A.beforeside_add_print_mpcode AS bef_add_print_mpcode";
        $query .= "\n        ,A.aftside_print_mpcode        AS aft_print_mpcode";
        $query .= "\n        ,A.aftside_add_print_mpcode    AS aft_add_print_mpcode";
        $query .= "\n        ,A.output_mpcode";
        //$query .= "\n        ,A.esti_detail_dvs_num";
        //$query .= "\n        ,A.paper_unitprice";
        //$query .= "\n        ,A.paper_machine_amt";
        //$query .= "\n        ,A.paper_R_amt";
        $query .= "\n        ,A.paper_price";
        //$query .= "\n        ,A.output";
        //$query .= "\n        ,A.output_unitprice";
        //$query .= "\n        ,A.output_machine_amt";
        //$query .= "\n        ,A.output_R_amt";
        $query .= "\n        ,A.output_price";
        //$query .= "\n        ,A.print";
        //$query .= "\n        ,A.print_unitprice";
        //$query .= "\n        ,A.print_machine_amt";
        //$query .= "\n        ,A.print_R_amt";
        $query .= "\n        ,A.print_price";
        $query .= "\n   FROM  esti_detail_brochure AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $conn->param($param["esti_seqno"]));

        $rs = $conn->Execute($query, [$param["esti_seqno"]]);

        return $rs;
    }

    /**
     * @brief 견적 상세별로 속한 후공정 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiInfoAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiInfoAfter */";
        $query .= "\n         A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.price";
        $query .= "\n        ,A.mpcode";
        $query .= "\n        ,A.info";
        $query .= "\n        ,A.detail";
        $query .= "\n   FROM  esti_after_history AS A";
        $query .= "\n  WHERE  A.esti_detail_dvs_num = %s";

        $query  = sprintf($query, $conn->param($param["esti_detail_dvs_num"]));

        return $conn->Execute($query, [$param["esti_detail_dvs_num"]]);
    }

    function updateEstiMatch($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE esti ";
        $query .= "\n     SET order_common_seqno = %s ";
        $query .= "\n  WHERE  esti_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"], $param["esti_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 사용자 견적 seqno 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiSeqno($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }
 
        $query  = "\n SELECT /* [EstiInfoDAO.inc] - selectEstiSeqno */";
        $query .= "\n        esti_seqno";
        $query .= "\n   FROM esti";
        $query .= "\n  WHERE esti_seqno IN (%s)";
        $query .= "\n    AND member_seqno = %s";

        $query = sprintf($query, $param["esti_seqno"]
                               , $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 사용자 견적 파일 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiFile($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }
 
        $query  = "\n SELECT  /* [EstiInfoDAO.inc] - selectEstiFileList */";
        $query .= "\n         file_path";
        $query .= "\n        ,save_file_name";
        $query .= "\n        ,origin_file_name";
        $query .= "\n        ,size";
        $query .= "\n        ,member_seqno";
        $query .= "\n   FROM  esti_file";
        $query .= "\n  WHERE  esti_seqno = %s";

        $query = sprintf($query, $conn->param($param["esti_seqno"]));

        return $conn->Execute($query, [$param["esti_seqno"]]);
    }

    /**
     * @brief 견적관련 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteEstiData($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n DELETE";
        $query .= "\n   FROM %s";
        $query .= "\n  WHERE esti_seqno = %s";

        $query  = sprintf($query, $param["table"]
                                , $conn->param($param["esti_seqno"]));

        return $conn->Execute($query, [$param["esti_seqno"]]);
    }
}
?>
