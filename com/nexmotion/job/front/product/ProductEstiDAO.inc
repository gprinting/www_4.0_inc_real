<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . '/common_dao/ProductCommonDAO.inc');

class ProductEstiDAO extends ProductCommonDAO {

    /**
     * @brief 견적_파일 정보 입력
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertEstiFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "esti_file";
        $temp["col"]["member_seqno"]       = $param["member_seqno"];
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        if ($this->blankParameterCheck($param, "esti_seqno")) {
            $temp["col"]["esti_seqno"] = $param["esti_seqno"];
        }

        return $this->insertData($conn, $temp);
    }

    function selectEstiFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $flag = false;

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.file_path";
        $query .= "\n        ,A.save_file_name";
        $query .= "\n        ,A.origin_file_name";
        $query .= "\n        ,A.esti_file_seqno";
        $query .= "\n        ,A.size";
        $query .= "\n   FROM  esti_file AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "esti_file_seqno")) {
            $query .= "\n    AND  A.esti_file_seqno   = ";
            $query .= $param["esti_file_seqno"];
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno       = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "esti_seqno")) {

            $query .= "\n    AND  A.esti_seqno = ";
            $query .= $param["esti_seqno"];
        }

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 견적_파일에 견적 일련번호 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateEstiFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]             = "esti_file";
        $temp["col"]["esti_seqno"] = $param["esti_seqno"];
        $temp["col"]["save_file_name"] = $param["save_file_name"];
        $temp["prk"]    = "esti_file_seqno";
        $temp["prkVal"] = $param["esti_file_seqno"];

        return $this->updateData($conn, $temp);
    }

    /**
     * @brief 견적_상세 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertEsti($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n INSERT INTO esti (";
        $query .= "\n    member_seqno";
        $query .= "\n   ,group_seqno";
        $query .= "\n   ,esti_num";
        $query .= "\n   ,cate_sortcode";
        $query .= "\n   ,title";
        $query .= "\n   ,esti_detail";
        $query .= "\n   ,amt";
        $query .= "\n   ,amt_note";
        $query .= "\n   ,page_cnt";
        $query .= "\n   ,amt_unit_dvs";
        $query .= "\n   ,count";
        $query .= "\n   ,expec_weight";
        $query .= "\n   ,state";
        $query .= "\n   ,memo";
        $query .= "\n   ,regi_date";
        $query .= "\n   ,origin_price";
        $query .= "\n   ,sale_rate";
        $query .= "\n   ,sale_price";
        $query .= "\n   ,esti_price";
        $query .= "\n   ,vat";
        $query .= "\n   ,order_price";
        $query .= "\n ) VALUES (";
        $query .= "\n    %s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,now()";
        $query .= "\n   ,0";
        $query .= "\n   ,0";
        $query .= "\n   ,0";
        $query .= "\n   ,0";
        $query .= "\n   ,0";
        $query .= "\n   ,0";
        $query .= "\n )";

        $query  = sprintf($query, $conn->param($param["member_seqno"])
                                , $conn->param($param["group_seqno"])
                                , $conn->param($param["esti_num"])
                                , $conn->param($param["cate_sortcode"])
                                , $conn->param($param["title"])
                                , $conn->param($param["esti_detail"])
                                , $conn->param($param["amt"])
                                , $conn->param($param["amt_note"])
                                , $conn->param($param["page_cnt"])
                                , $conn->param($param["amt_unit_dvs"])
                                , $conn->param($param["count"])
                                , $conn->param($param["expec_weight"])
                                , $conn->param($param["state"])
                                , $conn->param($param["memo"]));

        $bind = [];
        $bind[] = $param["member_seqno"];
        $bind[] = $param["group_seqno"];
        $bind[] = $param["esti_num"];
        $bind[] = $param["cate_sortcode"];
        $bind[] = $param["title"];
        $bind[] = $param["esti_detail"];
        $bind[] = $param["amt"];
        $bind[] = $param["amt_note"];
        $bind[] = $param["page_cnt"];
        $bind[] = $param["amt_unit_dvs"];
        $bind[] = $param["count"];
        $bind[] = $param["expec_weight"];
        $bind[] = $param["state"];
        $bind[] = $param["memo"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 견적_상세 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertEstiDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n INSERT INTO esti_detail (";
        $query .= "\n    esti_seqno";
        $query .= "\n   ,cate_sortcode";
        $query .= "\n   ,esti_detail_dvs_num";

        $query .= "\n   ,paper_info";
        $query .= "\n   ,paper_info_note";
        $query .= "\n   ,size_info";
        $query .= "\n   ,beforeside_tmpt_info";
        $query .= "\n   ,beforeside_tmpt_info_note";
        $query .= "\n   ,aftside_tmpt_info";
        $query .= "\n   ,aftside_tmpt_info_note";
        $query .= "\n   ,print_purp_info";
        $query .= "\n   ,print_purp_info_note";
        $query .= "\n   ,after_info";
        $query .= "\n   ,after_info_note";

        $query .= "\n   ,typ";
        $query .= "\n   ,work_size_wid";
        $query .= "\n   ,work_size_vert";
        $query .= "\n   ,cut_size_wid";
        $query .= "\n   ,cut_size_vert";
        $query .= "\n   ,tomson_size_wid";
        $query .= "\n   ,tomson_size_vert";
        $query .= "\n   ,paper_mpcode";
        $query .= "\n   ,beforeside_print_mpcode";
        $query .= "\n   ,beforeside_add_print_mpcode";
        $query .= "\n   ,aftside_print_mpcode";
        $query .= "\n   ,aftside_add_print_mpcode";
        $query .= "\n   ,output_mpcode";
        $query .= "\n   ,tot_tmpt";
        $query .= "\n   ,page_amt";
        $query .= "\n   ,expec_weight";
        $query .= "\n   ,after_use_yn";
        $query .= "\n   ,side_dvs";
        $query .= "\n ) VALUES (";
        $query .= "\n    %s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $conn->param($param["esti_seqno"])
                                , $conn->param($param["cate_sortcode"])
                                , $conn->param($param["esti_detail_dvs_num"])
                                , $conn->param($param["paper_info"])
                                , $conn->param($param["paper_info_note"])
                                , $conn->param($param["size_info"])
                                , $conn->param($param["beforeside_tmpt_info"])
                                , $conn->param($param["beforeside_tmpt_info_note"])
                                , $conn->param($param["aftside_tmpt_info"])
                                , $conn->param($param["aftside_tmpt_info_note"])
                                , $conn->param($param["print_purp_info"])
                                , $conn->param($param["print_purp_info_note"])
                                , $conn->param($param["after_info"])
                                , $conn->param($param["after_info_note"])
                                , $conn->param($param["typ"])
                                , $conn->param($param["work_size_wid"])
                                , $conn->param($param["work_size_vert"])
                                , $conn->param($param["cut_size_wid"])
                                , $conn->param($param["cut_size_vert"])
                                , $conn->param($param["tomson_size_wid"])
                                , $conn->param($param["tomson_size_vert"])
                                , $conn->param($param["paper_mpcode"])
                                , $conn->param($param["beforeside_print_mpcode"])
                                , $conn->param($param["beforeside_add_print_mpcode"])
                                , $conn->param($param["aftside_print_mpcode"])
                                , $conn->param($param["aftside_add_print_mpcode"])
                                , $conn->param($param["output_mpcode"])
                                , $conn->param($param["tot_tmpt"])
                                , $conn->param($param["page_amt"])
                                , $conn->param($param["expec_weight"])
                                , $conn->param($param["after_use_yn"])
                                , $conn->param($param["side_dvs"])
                                );

        $bind = [];
        $bind[] = $param["esti_seqno"];
        $bind[] = $param["cate_sortcode"];
        $bind[] = $param["esti_detail_dvs_num"];
        $bind[] = $param["paper_info"];
        $bind[] = $param["paper_info_note"];
        $bind[] = $param["size_info"];
        $bind[] = $param["beforeside_tmpt_info"];
        $bind[] = $param["beforeside_tmpt_info_note"];
        $bind[] = $param["aftside_tmpt_info"];
        $bind[] = $param["aftside_tmpt_info_note"];
        $bind[] = $param["print_purp_info"];
        $bind[] = $param["print_purp_info_note"];
        $bind[] = $param["after_info"];
        $bind[] = $param["after_info_note"];
        $bind[] = $param["typ"];
        $bind[] = $param["work_size_wid"];
        $bind[] = $param["work_size_vert"];
        $bind[] = $param["cut_size_wid"];
        $bind[] = $param["cut_size_vert"];
        $bind[] = $param["tomson_size_wid"];
        $bind[] = $param["tomson_size_vert"];
        $bind[] = $param["paper_mpcode"];
        $bind[] = $param["beforeside_print_mpcode"];
        $bind[] = $param["beforeside_add_print_mpcode"];
        $bind[] = $param["aftside_print_mpcode"];
        $bind[] = $param["aftside_add_print_mpcode"];
        $bind[] = $param["output_mpcode"];
        $bind[] = $param["tot_tmpt"];
        $bind[] = $param["page_amt"];
        $bind[] = $param["expec_weight"];
        $bind[] = $param["after_use_yn"];
        $bind[] = $param["side_dvs"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 견적_상세 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertEstiDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n INSERT INTO esti_detail_brochure (";
        $query .= "\n    esti_seqno";
        $query .= "\n   ,cate_sortcode";
        $query .= "\n   ,esti_detail_dvs_num";
        $query .= "\n   ,paper_info";
        $query .= "\n   ,paper_info_note";
        $query .= "\n   ,size_info";
        $query .= "\n   ,beforeside_tmpt_info";
        $query .= "\n   ,beforeside_tmpt_info_note";
        $query .= "\n   ,aftside_tmpt_info";
        $query .= "\n   ,aftside_tmpt_info_note";
        $query .= "\n   ,print_purp_info";
        $query .= "\n   ,print_purp_info_note";
        $query .= "\n   ,after_info";
        $query .= "\n   ,after_info_note";
        $query .= "\n   ,typ";
        $query .= "\n   ,work_size_wid";
        $query .= "\n   ,work_size_vert";
        $query .= "\n   ,cut_size_wid";
        $query .= "\n   ,cut_size_vert";
        $query .= "\n   ,tomson_size_wid";
        $query .= "\n   ,tomson_size_vert";
        $query .= "\n   ,cut_front_wing_size_wid";
        $query .= "\n   ,cut_front_wing_size_vert";
        $query .= "\n   ,work_front_wing_size_wid";
        $query .= "\n   ,work_front_wing_size_vert";
        $query .= "\n   ,cut_rear_wing_size_wid";
        $query .= "\n   ,cut_rear_wing_size_vert";
        $query .= "\n   ,work_rear_wing_size_wid";
        $query .= "\n   ,work_rear_wing_size_vert";
        $query .= "\n   ,seneca_size";
        $query .= "\n   ,paper_mpcode";
        $query .= "\n   ,beforeside_print_mpcode";
        $query .= "\n   ,beforeside_add_print_mpcode";
        $query .= "\n   ,aftside_print_mpcode";
        $query .= "\n   ,aftside_add_print_mpcode";
        $query .= "\n   ,output_mpcode";
        $query .= "\n   ,tot_tmpt";
        $query .= "\n   ,page_amt";
        $query .= "\n   ,expec_weight";
        $query .= "\n   ,after_use_yn";
        $query .= "\n   ,side_dvs";
        $query .= "\n ) VALUES (";
        $query .= "\n    %s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n   ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $conn->param($param["esti_seqno"])
                                , $conn->param($param["cate_sortcode"])
                                , $conn->param($param["esti_detail_dvs_num"])
                                , $conn->param($param["paper_info"])
                                , $conn->param($param["paper_info_note"])
                                , $conn->param($param["size_info"])
                                , $conn->param($param["beforeside_tmpt_info"])
                                , $conn->param($param["beforeside_tmpt_info_note"])
                                , $conn->param($param["aftside_tmpt_info"])
                                , $conn->param($param["aftside_tmpt_info_note"])
                                , $conn->param($param["print_purp_info"])
                                , $conn->param($param["print_purp_info_note"])
                                , $conn->param($param["after_info"])
                                , $conn->param($param["after_info_note"])
                                , $conn->param($param["typ"])
                                , $conn->param($param["work_size_wid"])
                                , $conn->param($param["work_size_vert"])
                                , $conn->param($param["cut_size_wid"])
                                , $conn->param($param["cut_size_vert"])
                                , $conn->param($param["tomson_size_wid"])
                                , $conn->param($param["tomson_size_vert"])
                                , $conn->param($param["cut_front_wing_size_wid"])
                                , $conn->param($param["cut_front_wing_size_vert"])
                                , $conn->param($param["work_front_wing_size_wid"])
                                , $conn->param($param["work_front_wing_size_vert"])
                                , $conn->param($param["cut_rear_wing_size_wid"])
                                , $conn->param($param["cut_rear_wing_size_vert"])
                                , $conn->param($param["work_rear_wing_size_wid"])
                                , $conn->param($param["work_rear_wing_size_vert"])
                                , $conn->param($param["seneca_size"])
                                , $conn->param($param["paper_mpcode"])
                                , $conn->param($param["beforeside_print_mpcode"])
                                , $conn->param($param["beforeside_add_print_mpcode"])
                                , $conn->param($param["aftside_print_mpcode"])
                                , $conn->param($param["aftside_add_print_mpcode"])
                                , $conn->param($param["output_mpcode"])
                                , $conn->param($param["tot_tmpt"])
                                , $conn->param($param["page_amt"])
                                , $conn->param($param["expec_weight"])
                                , $conn->param($param["after_use_yn"])
                                , $conn->param($param["side_dvs"])
                                );

        $bind = [];
        $bind[] = $param["esti_seqno"];
        $bind[] = $param["cate_sortcode"];
        $bind[] = $param["esti_detail_dvs_num"];
        $bind[] = $param["paper_info"];
        $bind[] = $param["paper_info_note"];
        $bind[] = $param["size_info"];
        $bind[] = $param["beforeside_tmpt_info"];
        $bind[] = $param["beforeside_tmpt_info_note"];
        $bind[] = $param["aftside_tmpt_info"];
        $bind[] = $param["aftside_tmpt_info_note"];
        $bind[] = $param["print_purp_info"];
        $bind[] = $param["print_purp_info_note"];
        $bind[] = $param["after_info"];
        $bind[] = $param["after_info_note"];
        $bind[] = $param["typ"];
        $bind[] = $param["work_size_wid"];
        $bind[] = $param["work_size_vert"];
        $bind[] = $param["cut_size_wid"];
        $bind[] = $param["cut_size_vert"];
        $bind[] = $param["tomson_size_wid"];
        $bind[] = $param["tomson_size_vert"];
        $bind[] = $param["cut_front_wing_size_wid"];
        $bind[] = $param["cut_front_wing_size_vert"];
        $bind[] = $param["work_front_wing_size_wid"];
        $bind[] = $param["work_front_wing_size_vert"];
        $bind[] = $param["cut_rear_wing_size_wid"];
        $bind[] = $param["cut_rear_wing_size_vert"];
        $bind[] = $param["work_rear_wing_size_wid"];
        $bind[] = $param["work_rear_wing_size_vert"];
        $bind[] = $param["seneca_size"];
        $bind[] = $param["paper_mpcode"];
        $bind[] = $param["beforeside_print_mpcode"];
        $bind[] = $param["beforeside_add_print_mpcode"];
        $bind[] = $param["aftside_print_mpcode"];
        $bind[] = $param["aftside_add_print_mpcode"];
        $bind[] = $param["output_mpcode"];
        $bind[] = $param["tot_tmpt"];
        $bind[] = $param["page_amt"];
        $bind[] = $param["expec_weight"];
        $bind[] = $param["after_use_yn"];
        $bind[] = $param["side_dvs"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 주문_후공정_내역 테이블에 데이터 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function insertEstiAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = [];
        $temp["table"] = "esti_after_history";

        $temp["col"]["esti_detail_dvs_num"] = $param["esti_detail_dvs_num"];
        $temp["col"]["esti_seqno"]   = $param["esti_seqno"];

        $temp["col"]["mpcode"] = $param["mpcode"];

        $temp["col"]["after_name"] = $param["after_name"];
        $temp["col"]["depth1"] = $param["depth1"];
        $temp["col"]["depth2"] = $param["depth2"];
        $temp["col"]["depth3"] = $param["depth3"];

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
    function insertEstiOptHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = array();
        $temp["table"] = "esti_opt_history";

        $temp["col"]["esti_seqno"]   = $param["esti_seqno"];

        $temp["col"]["mpcode"] = $param["mpcode"];

        $temp["col"]["opt_name"] = $param["opt_name"];
        $temp["col"]["depth1"] = $param["depth1"];
        $temp["col"]["depth2"] = $param["depth2"];
        $temp["col"]["depth3"] = $param["depth3"];

        $temp["col"]["detail"] = $param["detail"];
        $temp["col"]["info"]   = $param["info"];

        return $this->insertData($conn, $temp);
    }

    /**
     * @brief 견적 관련 데이터 삭제
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

        $query  = "DELETE";
        $query .= "  FROM %s";
        $query .= " WHERE esti_seqno = %s";

        $query  = sprintf($query, $param["table"]
                                , $conn->param($param["esti_seqno"]));

        $bind = [];
        $bind[] = $param["esti_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 견적_후공정_내역 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function deleteEstiAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $esti_detail_dvs_num = $this->arr2paramStr($conn,
                                                   $param["esti_detail_dvs_num"]);

        $query  = "DELETE";
        $query .= "  FROM esti_after_history";
        $query .= " WHERE esti_detail_dvs_num IN (%s)";

        $query  = sprintf($query, $esti_detail_dvs_num);

        return $conn->Execute($query);
    }

    /**
     * @brief 견적_파일 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리 실행 결과
     */
    function deleteEstiFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "DELETE";
        $query .= "  FROM esti_file";
        $query .= " WHERE esti_file_seqno = %s";
        
        $query  = sprintf($query, $conn->param($param["esti_file_seqno"]));

        $bind = [];
        $bind[] = $param["esti_file_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 견적파일 esti_seqno 비어있는 것 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function selectEstiFileEmpty($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "SELECT  esti_file_seqno";
        $query .= "       ,file_path";
        $query .= "       ,save_file_name";
        $query .= "  FROM  esti_file";
        $query .= " WHERE  member_seqno = %s";
        $query .= "   AND  esti_seqno IS null";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        return $conn->Execute($query, [$param["member_seqno"]]);
    }

    /**
     * @brief 시퀀스 증가용 마지막 주문번호 검색
     *
     * @param $conn  = connection identifer
     *
     * @return 마지막 숫자
     */
    function selectEstiLastNum($conn) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $today = date("Y-m-d");

        $query  = "\n   SELECT /* ProductEstiDAO.inc - 마지막 견적번호 검색 */";
        $query .= "\n          esti_num";
        $query .= "\n     FROM esti";
        $query .= "\n    WHERE '%s 00:00:00' <= regi_date";
        $query .= "\n      AND regi_date <= '%s 23:59:59'";
        $query .= "\n ORDER BY esti_seqno DESC";
        $query .= "\n    LIMIT 1";

        $query  = sprintf($query, $today, $today);

        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $last_num = 1;
        } else {
            $last_num = intval(substr($rs->fields["esti_num"], 11)) + 1;
        }

        return $last_num;
    }
}
?>
