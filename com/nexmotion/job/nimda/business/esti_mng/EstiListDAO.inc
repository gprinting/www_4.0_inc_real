<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/business/esti_mng/EstiListHTML.inc');

/**
 * @file MemberCommonListDAO.inc
 *
 * @brief 영업 - 견적관리 - 견적리스트 DAO
 */

class EstiListDAO extends BusinessCommonDAO {
    
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
    function selectEstiList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        
        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiList, 견적 리스트 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         B.member_name";
        $query .= "\n        ,A.esti_seqno";
        $query .= "\n        ,A.state";
        $query .= "\n        ,A.regi_date";
        $query .= "\n        ,A.esti_date";
        $query .= "\n        ,A.esti_price";
        $query .= "\n        ,A.order_price";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.esti_detail";
        $query .= "\n        ,A.esti_mng";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n   FROM  esti AS A";
        $query .= "\n        ,member AS B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.regi_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.regi_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  A.title LIKE '%%";
            $query .= substr($param["title"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "office_nick")) {
            $query .= "\n    AND  B.office_nick LIKE '%";
            $query .= substr($param["office_nick"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_tel")) {
            $query .= "\n    AND  B.tel_num LIKE '%";
            $query .= substr($param["member_tel"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_cell")) {
            $query .= "\n    AND  B.cell_num LIKE '%";
            $query .= substr($param["member_cell"], 1, -1);
            $query .= "%'";
        }

        $query .= "\n  ORDER BY A.esti_seqno DESC";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $rs = $conn->Execute($query);

        return $rs;
    }


    /**
     * @brief 견적리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberGrade($conn, $param) {
        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query  = "\n SELECT  /* 견적 회원등급 쿼리*/";
        $query .= "\n         A.grade_name";
        $query .= "\n   FROM  member_grade_policy AS A";
        $query .= "\n        ,member AS B";
        $query .= "\n  WHERE  A.grade = B.grade";
        $query .= "\n    AND  B.member_seqno = %s";
        $query  = sprintf($query, $param);

        $rs = $conn->Execute($query);

        return $rs;

    }

    /**
     * @brief 견적리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiFlattypYn($conn, $high_sortcode) {
        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $high_sortcode = $this->parameterEscape($conn, $high_sortcode);

        $query  = "\n SELECT  /* 견적 회원등급 쿼리*/";
        $query .= "\n         A.sortcode";
        $query .= "\n        ,A.flattyp_yn";
        $query .= "\n   FROM  cate AS A";
        $query .= "\n  WHERE  A.high_sortcode = %s";
        $query  = sprintf($query, $high_sortcode);

        $rs = $conn->Execute($query);

        return $rs;

    }

    /**
     * @brief 견적 기본정보
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

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiBaseInfo, 견적기본정보 */";
        $query .= "\n         A.title";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.amt_note";
        $query .= "\n        ,A.page_cnt";
        $query .= "\n        ,A.state";
        $query .= "\n        ,A.esti_mng";
        $query .= "\n        ,A.memo";
        $query .= "\n   FROM  esti AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;

    }

    /**
     * @brief 견적 상세(낱장형)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiDetail($conn, $param) {
        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiDetail, 견적 상세 쿼리(낱장형) */";
        $query .= "\n         A.paper_info";
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
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n   FROM  esti_detail AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;

    }

    /**
     * @brief 견적 상세(책자형)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiDetailBrochure($conn, $param) {
        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiDetailBrochure, 견적 상세 쿼리(책자형) */";
        $query .= "\n         A.paper_info";
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
        $query .= "\n        ,A.page_amt";
        $query .= "\n        ,A.page_note";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n   FROM  esti_detail_brochure AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;

    }

    /**
     * @brief 견적별로 속한 후공정 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiAfterHistory, 견적 후공정 검색*/";
        $query .= "\n         A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.esti_after_history_seqno";
        $query .= "\n        ,A.mpcode";
        $query .= "\n   FROM  esti_after_history AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "esti_seqno")) {
            $query .= "\n    AND  A.esti_seqno = ";
            $query .= $param["seqno_seqno"];
        }
        if ($this->blankParameterCheck($param, "esti_detail_dvs_num")) {
            $query .= "\n    AND  A.esti_detail_dvs_num = ";
            $query .= $param["esti_detail_dvs_num"];
        }
        if ($this->blankParameterCheck($param, "after_name")) {
            $query .= "\n    AND  A.after_name = ";
            $query .= $param["after_name"];
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 견적 상태 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateEstiState($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  esti ";
        $query .= "\n    SET  state = %s ";
        $query .= "\n        ,esti_mng = %s ";
        $query .= "\n  WHERE  esti_seqno = %s ";

        $query = sprintf($query, $param["state"]
                               , $param["esti_mng"]
                               , $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 견적 가격 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateEstiPrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  esti ";
        $query .= "\n    SET  origin_price = %s ";
        $query .= "\n        ,sale_rate = %s ";
        $query .= "\n        ,sale_price = %s ";
        $query .= "\n        ,esti_price = %s ";
        $query .= "\n        ,vat = %s ";
        $query .= "\n        ,order_price = %s ";
        $query .= "\n        ,memo = %s ";
        $query .= "\n  WHERE  esti_seqno = %s ";

        $query = sprintf($query, $param["origin_price"]
                               , $param["sale_rate"]
                               , $param["sale_price"]
                               , $param["esti_price"]
                               , $param["vat"]
                               , $param["order_price"]
                               , $param["memo"]
                               , $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 견적 상세 낱장 가격 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateEstiDetailPrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  esti_detail";
        $query .= "\n    SET  paper = %s ";
        $query .= "\n        ,paper_tmpt = %s ";
        $query .= "\n        ,paper_unitprice = %s ";
        $query .= "\n        ,paper_machine_amt = %s ";
        $query .= "\n        ,paper_R_amt = %s ";
        $query .= "\n        ,paper_price = %s ";
        $query .= "\n        ,paper_note = %s ";

        $query .= "\n        ,output = %s ";
        $query .= "\n        ,output_tmpt = %s ";
        $query .= "\n        ,output_unitprice = %s ";
        $query .= "\n        ,output_machine_amt = %s ";
        $query .= "\n        ,output_R_amt = %s ";
        $query .= "\n        ,output_price = %s ";
        $query .= "\n        ,output_note = %s ";

        $query .= "\n        ,print = %s ";
        $query .= "\n        ,print_tmpt = %s ";
        $query .= "\n        ,print_unitprice = %s ";
        $query .= "\n        ,print_machine_amt = %s ";
        $query .= "\n        ,print_R_amt = %s ";
        $query .= "\n        ,print_price = %s ";
        $query .= "\n        ,print_note = %s ";
        $query .= "\n  WHERE  esti_seqno = %s ";

        $query = sprintf($query, $param["paper"]
                               , $param["paper_tmpt"]
                               , $param["paper_unitprice"]
                               , $param["paper_machine_amt"]
                               , $param["paper_R_amt"]
                               , $param["paper_price"]
                               , $param["paper_note"]

                               , $param["output"]
                               , $param["output_tmpt"]
                               , $param["output_unitprice"]
                               , $param["output_machine_amt"]
                               , $param["output_R_amt"]
                               , $param["output_price"]
                               , $param["output_note"]

                               , $param["print"]
                               , $param["print_tmpt"]
                               , $param["print_unitprice"]
                               , $param["print_machine_amt"]
                               , $param["print_R_amt"]
                               , $param["print_price"]
                               , $param["print_note"]

                               , $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 견적 상세 책자 가격 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateEstiDetailBrochurePrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  esti_detail_brochure";
        $query .= "\n    SET  paper = %s ";
        $query .= "\n        ,paper_tmpt = %s ";
        $query .= "\n        ,paper_unitprice = %s ";
        $query .= "\n        ,paper_machine_amt = %s ";
        $query .= "\n        ,paper_R_amt = %s ";
        $query .= "\n        ,paper_price = %s ";
        $query .= "\n        ,paper_note = %s ";

        $query .= "\n        ,output = %s ";
        $query .= "\n        ,output_tmpt = %s ";
        $query .= "\n        ,output_unitprice = %s ";
        $query .= "\n        ,output_machine_amt = %s ";
        $query .= "\n        ,output_R_amt = %s ";
        $query .= "\n        ,output_price = %s ";
        $query .= "\n        ,output_note = %s ";

        $query .= "\n        ,print = %s ";
        $query .= "\n        ,print_tmpt = %s ";
        $query .= "\n        ,print_unitprice = %s ";
        $query .= "\n        ,print_machine_amt = %s ";
        $query .= "\n        ,print_R_amt = %s ";
        $query .= "\n        ,print_price = %s ";
        $query .= "\n        ,print_note = %s ";
        $query .= "\n  WHERE  esti_seqno = %s ";
        $query .= "\n    AND  typ = %s ";

        $query = sprintf($query, $param["paper"]
                               , $param["paper_tmpt"]
                               , $param["paper_unitprice"]
                               , $param["paper_machine_amt"]
                               , $param["paper_R_amt"]
                               , $param["paper_price"]
                               , $param["paper_note"]

                               , $param["output"]
                               , $param["output_tmpt"]
                               , $param["output_unitprice"]
                               , $param["output_machine_amt"]
                               , $param["output_R_amt"]
                               , $param["output_price"]
                               , $param["output_note"]

                               , $param["print"]
                               , $param["print_tmpt"]
                               , $param["print_unitprice"]
                               , $param["print_machine_amt"]
                               , $param["print_R_amt"]
                               , $param["print_price"]
                               , $param["print_note"]

                               , $param["esti_seqno"]
                               , $param["typ"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 견적 후공정 가격 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateEstiAfterPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n UPDATE  esti_after_history";
        $query .= "\n    SET  unitprice = %s ";
        $query .= "\n        ,machine_amt = %s ";
        $query .= "\n        ,R_amt = %s ";
        $query .= "\n        ,price = %s ";
        $query .= "\n        ,note = %s ";
        $query .= "\n  WHERE  esti_after_history_seqno = %s ";

        $query = sprintf($query, $param["unitprice"]
                               , $param["machine_amt"]
                               , $param["R_amt"]
                               , $param["price"]
                               , $param["note"]
                               , $param["esti_after_history_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 관리자 견적 파일 리스트 
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
 
        $query .= "\n SELECT  esti_file_seqno";
        $query .= "\n        ,file_path";
        $query .= "\n        ,save_file_name";
        $query .= "\n        ,origin_file_name";
        $query .= "\n   FROM  esti_file";
        $query .= "\n  WHERE  esti_seqno = %s";

        $query = sprintf($query, $param["esti_seqno"]);

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 견적에 사용할 상품종이정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $dvs   = 구분값
     *
     * @return 검색결과
     */
    function selectPrdtPaper($conn, $param, $dvs) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtPaper, 상품 종이 검색 */";
        switch($dvs) {
            case "sort" :
                $query .= "\n         DISTINCT";
                $query .= "\n         A.sort";
                break;
            case "name" :
                $query .= "\n         DISTINCT";
                $query .= "\n         A.name";
                break;
            case "info" :
                $query .= "\n         A.dvs";
                $query .= "\n        ,A.color";
                $query .= "\n        ,A.basisweight";
                $query .= "\n        ,A.basisweight_unit";
                $query .= "\n        ,A.affil";
                $query .= "\n        ,A.mpcode";
                break;
        }

        $query .= "\n   FROM  prdt_paper AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "search_check")) {
            $query .= "\n    AND  A.search_check like '";
            $query .= substr($param["search_check"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "sort")) {
            $query .= "\n    AND  A.sort = ";
            $query .= $param["sort"];
        }
        if ($this->blankParameterCheck($param, "name")) {
            $query .= "\n    AND  A.name = ";
            $query .= $param["name"];
        }
        if ($this->blankParameterCheck($param, "affil")) {
            $query .= "\n    AND  A.affil = ";
            $query .= $param["affil"];
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 견적에 사용할 상품출력정보 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function selectPrdtOutputInfo($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtOutputInfo, 상품출력정보 검색 */";
        $query .= "\n         A.output_name";
        $query .= "\n        ,A.mpcode";
        $query .= "\n        ,A.output_board_dvs";
        $query .= "\n   FROM  prdt_output_info AS A";

        return $conn->Execute($query);
    }

    /**
     * @brief 견적에 사용할 상품인쇄정보 검색
     *
     * @param $conn       = connection identifier
     * @param $sortcode_m = 카테고리 중분류 코드
     *
     * @return 검색결과
     */
    function selectPrdtPrintInfo($conn, $sortcode_m) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $sortcode_m = $this->parameterEscape($conn, $sortcode_m);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtPrintInfo, 상품인쇄정보 검색 */";
        $query .= "\n         A.print_name";
        $query .= "\n        ,A.mpcode";
        $query .= "\n        ,A.purp_dvs";
        $query .= "\n   FROM  prdt_print_info AS A";
        $query .= "\n  WHERE  cate_sortcode = %s";

        $query  = sprintf($query, $sortcode_m);

        return $conn->Execute($query);
    }

    /**
     * @brief 견적에 선택한 종이정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCatePaper($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectCatePaper, 카테고리 종이 검색 */";
        $query .= "\n         A.sort";
        $query .= "\n        ,A.name";
        $query .= "\n        ,A.dvs";
        $query .= "\n        ,A.color";
        $query .= "\n        ,A.basisweight";
        $query .= "\n   FROM  cate_paper  AS A";
        $query .= "\n  WHERE  A.mpcode = %s";

        $query  = sprintf($query, $param["mpcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 견적 상세 mpcode만 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiDetailMpcode($conn, $param, $table) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiDetailMpcode, 견적 상세 mpcode 검색 */";
        $query .= "\n         A.paper_mpcode";
        $query .= "\n        ,A.beforeside_print_mpcode AS bef_print_mpcode";
        $query .= "\n        ,A.beforeside_add_print_mpcode AS bef_add_print_mpcode";
        $query .= "\n        ,A.aftside_print_mpcode AS aft_print_mpcode";
        $query .= "\n        ,A.aftside_add_print_mpcode AS aft_add_print_mpcode";
        $query .= "\n        ,A.output_mpcode";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.esti_detail_dvs_num";
        $query .= "\n   FROM  %s AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";

        $query  = sprintf($query, $table, $param["esti_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 견적 상세 구분번호만 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiDetailDvsNum($conn, $param, $table) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectEstiDetailDvsNum, 견적 상세 구분번호 검색 */";
        $query .= "\n         A.esti_detail_dvs_num";
        $query .= "\n   FROM  %s AS A";
        $query .= "\n  WHERE  A.esti_seqno = %s";
        $query .= "\n    AND  A.typ = %s";

        $query  = sprintf($query, $table, $param["esti_seqno"]
                                        , $param["typ"]);

        $rs = $conn->Execute($query);

        return $rs->fields["esti_detail_dvs_num"];
    }

    /**
     * @brief 종이 단가 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtPaperPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtPaperPrice, 종이 단가 검색 */";
        $query .= "\n         A.sell_price";
        $query .= "\n   FROM  prdt_paper_price AS A";
        $query .= "\n  WHERE  A.prdt_paper_mpcode = %s";

        $query  = sprintf($query, $conn->param($param["prdt_paper_mpcode"]));

        $rs = $conn->Execute($query, [$param["prdt_paper_mpcode"]]);

        return $rs->fields["sell_price"];
    } 

    /**
     * @brief 출력 단가 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtOutputPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtOutputPrice, 출력 단가 검색 */";
        $query .= "\n         A.sell_price";
        $query .= "\n   FROM  prdt_stan_price AS A";
        $query .= "\n  WHERE  A.prdt_output_info_mpcode = %s";

        $query  = sprintf($query, $conn->param($param["prdt_output_info_mpcode"]));

        $rs = $conn->Execute($query, [$param["prdt_output_info_mpcode"]]);

        return $rs->fields["sell_price"];
    } 

    /**
     * @brief 인쇄 단가 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtPrintPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectPrdtPrintPrice, 인쇄 단가 검색 */";
        $query .= "\n         A.sell_price";
        $query .= "\n   FROM  prdt_print_price AS A";
        $query .= "\n  WHERE  A.prdt_print_info_mpcode = %s";
        $query .= "\n    AND  A.amt >= %s";
        $query .= "\n  ORDER  BY amt + 0";
        $query .= "\n  LIMIT  1";

        $query  = sprintf($query, $conn->param($param["prdt_print_info_mpcode"])
                                , $conn->param($param["amt"]));

        $rs = $conn->Execute($query, [
            $param["prdt_print_info_mpcode"],
            $param["amt"]
        ]);

        return $rs->fields["sell_price"];
    } 

    /**
     * @brief 상품 후공정 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterInfo($conn, $param, $field) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectAfterInfo, 상품 후공정 검색 */";
        $query .= "\n         %s";
        $query .= "\n   FROM  prdt_after AS A";
        $query .= "\n        ,cate_after AS B";
        $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "after_name")) {
            $query .= "\n    AND  A.after_name = ";
            $query .= $param["after_name"];
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = ";
            $query .= $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = ";
            $query .= $param["depth2"];
        }

        $query  = sprintf($query, $field, $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs;
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
        if (!$this->connectionCheck($conn)) {
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
     * @brief 주문_후공정_내역 데이터 삭제
     *
     * @param $conn  = connection identifier
     * @param $seqno = 후공정 내역 일련번호
     *
     * @return 쿼리실행결과
     */
    function deleteEstiAfterHistory($conn, $seqno) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $temp = [];
        $temp["table"] = "esti_after_history";

        $temp["prk"]    = "esti_after_history_seqno";
        $temp["prkVal"] = $seqno;

        return $this->deleteData($conn, $temp);
    }

    /**
     * @brief 주문_후공정_내역 중복검색
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return 쿼리실행결과
     */
    function selectDupEstiAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectDupEstiAfterHistory, 견적 후공정 중복값 검색 */";
        $query .= "        1";
        $query .= "  FROM  esti_after_history";
        $query .= " WHERE  esti_detail_dvs_num = %s";
        $query .= "   AND  mpcode = %s";

        $query  = sprintf($query, $conn->param($param["esti_detail_dvs_num"])
                                , $conn->param($param["mpcode"]));

        $temp = [
            $param["esti_detail_dvs_num"]
            ,$param["mpcode"]
        ];

        return $conn->Execute($query, $temp)->fields;
    }

    /**
     * @brief 카테고리 후공정 단가 검색
     *
     * @param $conn  = connection identifer
     *
     * @return 마지막 숫자
     */
    function selectCateAfterPrice($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false; 
        }

        $query  = "\n SELECT  /* [EstiListDAO.inc] - selectCateAfterPrice, 견적 후공정 중복값 검색 */";
        $query .= "        cate_after_mpcode AS mpcode";
        $query .= "       ,sell_price";
        $query .= "  FROM  cate_after_price";
        $query .= " WHERE  cate_after_mpcode IN (%s)";
        $query .= "   AND  amt = '1'";

        $query  = sprintf($query, $param["mpcode"]);

        return $conn->Execute($query);
    }
}
?>
