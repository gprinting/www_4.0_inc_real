<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/output_mng/OutputMngHtml.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/manufacture/output_mng/OutputMngDOC.inc');

/**
 * @file OutputListDAO.inc
 *
 * @brief 생산 - 출력관리 - 출력리스트 DAO
 */
class OutputListDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 생산공정관리 - 출력
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOutputList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $preset_cate = $param["preset_cate"];
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  DISTINCT(A.typset_num) ";
            $query .= "\n       ,A.sheet_typset_seqno ";
            $query .= "\n       ,A.print_title ";
            $query .= "\n       ,A.paper_name ";
            $query .= "\n       ,A.print_amt ";
            $query .= "\n       ,A.beforeside_tmpt ";
            $query .= "\n       ,A.aftside_tmpt ";
            $query .= "\n       ,A.prdt_page ";
            $query .= "\n       ,B.erp_state_name ";
            $query .= "\n       ,A.print_amt_unit ";
            $query .= "\n       ,A.specialty_items ";
            $query .= "\n       ,A.memo ";
            $query .= "\n       ,E.sheet_typset_seqno AS printed";
            $query .= "\n       ,CONCAT(C.file_path, C.save_file_name) as typset_pdf  ";
            $query .= "\n       ,CONCAT(D.file_path, D.save_file_name) as typset_label ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  INNER JOIN  state_admin AS B ON A.state = B.state_code ";
        $query .= "\n  LEFT JOIN  sheet_typset_file AS C ON A.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  bylabel_print_record AS E ON A.sheet_typset_seqno = E.sheet_typset_seqno AND E.empl_seqno = " . $_SESSION["empl_seqno"] . " AND E.page = " . $param["page"];
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"category")) {
            $val = "";
            if($param["category"] == "'11000'") {
                $val = "명함";
            }
            if($param["category"] == "'14000'") {
                $val = "스티커";
            }
            if($param["category"] == "'7000'") {
                $val = "봉투";
            }
            if($param["category"] == "'8000'") {
                $val = "전단";
            }
            if($param["category"] == "'9000'") {
                $val = "독판";
            }
            if($param["category"] == "'5000'") {
                $val = "디지털";
            }
            $query .= "\n   AND  A.dvs LIKE '%" . $val . "%' ";
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2320'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A.regi_date > " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A.regi_date <= " . $param["to"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.name LIKE '% " . $val . "%' ";
        }

        $query .= "\nORDER BY A.typset_num DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);
    }

    /**
     * @brief 생산공정관리 - 출력
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOutputList2($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $preset_cate = $param["preset_cate"];
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  DISTINCT(A.typset_num) ";
            $query .= "\n       ,A.sheet_typset_seqno ";
            $query .= "\n       ,A.print_title ";
            $query .= "\n       ,A.paper_name ";
            $query .= "\n       ,A.print_amt ";
            $query .= "\n       ,A.beforeside_tmpt ";
            $query .= "\n       ,A.aftside_tmpt ";
            $query .= "\n       ,A.prdt_page ";
            $query .= "\n       ,B.erp_state_name ";
            $query .= "\n       ,A.print_amt_unit ";
            $query .= "\n       ,A.specialty_items ";
            $query .= "\n       ,A.memo ";
            $query .= "\n       ,E.sheet_typset_seqno AS printed";
            $query .= "\n       ,CONCAT(C.file_path, C.save_file_name) as typset_pdf  ";
            $query .= "\n       ,CONCAT(D.file_path, D.save_file_name) as typset_label ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  INNER JOIN  state_admin AS B ON A.state = B.state_code ";
        $query .= "\n  LEFT JOIN  sheet_typset_file AS C ON A.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  bylabel_print_record AS E ON A.sheet_typset_seqno = E.sheet_typset_seqno AND E.empl_seqno = " . $_SESSION["empl_seqno"] . " AND E.page = " . $param["page"];
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"category")) {
            $val = "";
            if($param["category"] == "'11000'") {
                $val = "명함";
            }
            if($param["category"] == "'14000'") {
                $val = "스티커";
            }
            if($param["category"] == "'7000'") {
                $val = "봉투";
            }
            if($param["category"] == "'8000'") {
                $val = "전단";
            }
            if($param["category"] == "'9000'") {
                $val = "독판";
            }
            if($param["category"] == "'5000'") {
                $val = "디지털";
            }
            $query .= "\n   AND  A.dvs LIKE '%" . $val . "%' ";
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2320'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A.regi_date > " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A.regi_date <= " . $param["to"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.name LIKE '% " . $val . "%' ";
        }

        $query .= "\nORDER BY A.typset_num DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

       /* if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        } */
        return $conn->Execute($query);
    }

    /**
     * @brief 생산공정관리 - 출력
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOutputProcess($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.typset_num ";
            $query .= "\n       ,A.sheet_typset_seqno ";
            $query .= "\n       ,A.print_title ";
            $query .= "\n       ,A.paper_name ";
            $query .= "\n       ,A.print_amt ";
            $query .= "\n       ,A.beforeside_tmpt ";
            $query .= "\n       ,A.aftside_tmpt ";
            $query .= "\n       ,A.prdt_page ";
            $query .= "\n       ,B.erp_state_name ";
            $query .= "\n       ,A.print_amt_unit ";
            $query .= "\n       ,A.specialty_items ";
            $query .= "\n       ,A.memo ";
            $query .= "\n       ,E.state_modi_date ";
            $query .= "\n       ,CONCAT(C.file_path, C.save_file_name) as typset_pdf  ";
            $query .= "\n       ,CONCAT(D.file_path, D.save_file_name) as typset_label ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  INNER JOIN  state_admin AS B ON A.state = B.state_code ";
        $query .= "\n  LEFT JOIN  sheet_typset_file AS C ON A.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  typset_state_history AS E ON E.sheet_typset_seqno = A.sheet_typset_seqno AND E.state = '2320' ";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $query .= "\n   AND  A.print_etprs = " . $param["preset_cate"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  B.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2280'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }

        if($param["state"] == "'2220'" || $param["state"] == "'2280'") {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date <= " . $param["to"];
            }
        }

        $query .= "\nGROUP BY A.typset_num ";
        if($param["state"] == "'2280'") {

            $query .= "\nORDER BY E.state_modi_date DESC ";
            $query .= "\nLIMIT 200 ";
            //$query .= "\nLIMIT 30 ";
        }

        return $conn->Execute($query);
    }

    function findProducePlace($conn, $keyword) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    select cpn_name ";
        $query .= "\n   from extnl_etprs ";
        $query .= "\n   where pur_prdt = '인쇄' ";

        $rs =  $conn->Execute($query);

        $presets = array();
        while ($rs && !$rs->EOF) {
            $preset = $rs->fields["cpn_name"];
            array_push($presets, $preset);
            $rs->MoveNext();
        }

        return $presets;
    }

    function selectOriginLabelURL($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT file_path, save_file_name ";
        $query .= "\n   FROM sheet_typset_label_file ";
        $query .= "\n   WHERE sheet_typset_seqno = " . $param['sheet_typset_seqno'];

        $rs = $conn->Execute($query);

        if($rs != null) {
            return $rs->fields["file_path"] .  $rs->fields["save_file_name"];
        } else {
            return null;
        }
    }

    function selectOutputLabelView($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT file_path, save_file_name ";
        $query .= "\n   FROM sheet_typset_label_file ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $param["sheet_typset_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 출력공정 상세 보기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOutputDetailView($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\nSELECT A.sheet_typset_seqno ";
        $query .= "\n       ,A.typset_num ";
        $query .= "\n       ,A.print_title ";
        $query .= "\n       ,B.worker_id AS orderer ";
        $query .= "\n       ,A.op_typ AS typ ";
        $query .= "\n       ,A.op_typ_detail AS typ_detail ";
        $query .= "\n       ,B.subpaper ";
        $query .= "\n       ,B.wid_size ";
        $query .= "\n       ,B.vert_size ";
        $query .= "\n       ,B.affil ";
        $query .= "\n       ,A.memo ";
        $query .= "\n       ,A.state ";
        $query .= "\n       ,A.print_etprs ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN typset_format AS B ON A.typset_format_seqno = B.typset_format_seqno ";
        $query .= "\n WHERE  1 = 1 ";

        //일련번호 
        if ($this->blankParameterCheck($param ,"seqno")) {
            $query .= "\n   AND  A.sheet_typset_seqno = " . $param["seqno"];
        }

        //조판번호
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }

        return $conn->Execute($query);
    }

    function selectOrderDetailFileNumFromSheetTypsetSeqno($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT order_detail_file_num ";
        $query .= "\n   FROM sheet_typset AS A";
        $query .= "\n   INNER JOIN amt_order_detail_sheet AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno";
        $query .= "\n   INNER JOIN order_detail_count_file AS C ON B.order_detail_count_file_seqno = C.order_detail_count_file_seqno ";
        $query .= "\n   WHERE A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    function selectLabelInfo($conn, $param_sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT C.order_common_seqno, C.cate_sortcode ,D.member_name, E.dlvr_way, E.invo_cpn, C.title, D.dlvr_add_info, C.order_detail , C.page_cnt, C.count, F.cut_size_wid, F.cut_size_vert, C.order_num, C.work_memo, A.typset_num, G.preview_file_path, G.preview_file_name FROM sheet_typset AS A ";
        $query .= "\n    INNER JOIN order_typset AS B ON A.typset_num = B.typset_num  ";
        $query .= "\n    INNER JOIN order_common AS C ON B.order_num = C.order_num  ";
        $query .= "\n    INNER JOIN member AS D ON C.member_seqno = D.member_seqno  ";
        $query .= "\n    LEFT JOIN order_dlvr AS E ON C.order_common_seqno = E.order_common_seqno AND E.tsrs_dvs = '수신'  ";
        $query .= "\n    INNER JOIN order_detail AS F ON C.order_common_seqno = F.order_common_seqno  ";
        $query .= "\n    INNER JOIN order_file AS G ON C.order_common_seqno = G.order_common_seqno  ";
        $query .= "\n    WHERE A.sheet_typset_seqno IN (%s)  ";
        $query .= "\n    GROUP BY C.order_common_seqno  ";
        $query .= "\n    ORDER BY B.typset_num  ";
        $query = sprintf($query, $param_sheet_typset_seqno);

        return $conn->Execute($query);
    }

    function selectOrderAfterInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name, ' - ' ,detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND basic_yn='N' ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectOrderAfterInfo2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND basic_yn='N' ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectOrderAfterInfoByOrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(A.after_name, ' - ' ,A.detail) order by A.after_name desc SEPARATOR ' / ') as after_detail, B.cate_sortcode ";
        $query .= "\n       FROM order_after_history AS A ";
        $query .= "\n       INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n       WHERE B.order_num = %s  ";
        $query .= "\n       AND (A.basic_yn='N' OR A.after_name = '접지') ";
        $query .= "\n       GROUP BY A.order_common_seqno ";
        $query .= "\n       ORDER BY A.after_name asc ";

        $query  = sprintf($query, $param["order_num"]);
        $result = $conn->Execute($query);
        return $result->fields["after_detail"];
    }

    function selectSeqnoByTypsetNum($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT sheet_typset_seqno FROM sheet_typset WHERE typset_num = '%s'  ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query)->fields["sheet_typset_seqno"];
    }

    function selectOrderNumInTypset($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT DISTINCT B.order_num, A.typset_num, D.dlvr_way FROM sheet_typset  AS A  ";
        $query .= "\n    INNER JOIN order_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n    INNER JOIN order_common AS C ON B.order_num = C.order_num ";
        $query .= "\n    INNER JOIN order_dlvr AS D ON C.order_common_seqno = D.order_common_seqno ";
        $query .= "\n   WHERE A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    function selectTypsetAllAfterNum($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT * FROM typset_state_history AS A  ";
        $query .= "\n   WHERE A.sheet_typset_seqno = %s AND state = '2620' ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    function selectTypsetCompleteAfterNum($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT COUNT(DISTINCT detail) as cnt ";
        $query .= "\n   FROM typset_state_history ";
        $query .= "\n   WHERE sheet_typset_seqno = %s AND state = '2680' ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    function selectOrderNumInTypsetForPrint($conn, $sheet_typset_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT DISTINCT B.order_num, D.*  ";
        $query .= "\n    FROM sheet_typset  AS A ";
        $query .= "\n    INNER JOIN order_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n    INNER JOIN order_common AS C ON B.order_num = C.order_num ";
        $query .= "\n    LEFT JOIN order_after_history AS D ON C.order_common_seqno = D.order_common_seqno ";
        $query .= "\n   WHERE A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    /**
     * @brief 생산지시 인쇄뷰
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectProduceOrdPrint($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query = "\nSELECT C.sell_site";
        $query .= "\n      ,D.member_name";
        $query .= "\n      ,A.title";
        $query .= "\n      ,B.work_size_wid";
        $query .= "\n      ,B.work_size_vert";
        $query .= "\n      ,B.tot_tmpt";
        $query .= "\n      ,B.amt";
        $query .= "\n      ,B.amt_unit_dvs";
        $query .= "\n      ,A.page_cnt";
        $query .= "\n      ,A.work_memo";
        $query .= "\n      ,E.invo_cpn";
        $query .= "\n      ,A.order_mng";
        $query .= "\n      ,H.name";
        $query .= "\n      ,A.cate_sortcode";
        $query .= "\n      ,A.order_detail";
        $query .= "\n      ,A.count";
        $query .= "\n  FROM order_common AS A";
        $query .= "\n  INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n  INNER JOIN cpn_admin AS C ON A.cpn_admin_seqno = C.cpn_admin_seqno";
        $query .= "\n  INNER JOIN member AS D ON A.member_seqno = D.member_seqno";
        $query .= "\n  LEFT JOIN order_dlvr AS E ON A.order_common_seqno = E.order_common_seqno AND E.tsrs_dvs = '수신'";
        $query .= "\n  INNER JOIN order_state_history AS F ON A.order_common_seqno = F.order_common_seqno AND F.state = '1380'";
        $query .= "\n  LEFT JOIN empl AS G ON F.empl_id = G.empl_id ";
        $query .= "\n  LEFT JOIN empl_human_info AS H ON G.empl_seqno = H.empl_seqno ";
        $query .= "\n WHERE A.order_common_seqno = B.order_common_seqno";
        if ($this->blankParameterCheck($param ,"order_common_seqno")) {
            $query .= "\n   AND  A.order_common_seqno = " . $param["order_common_seqno"];
        }
        if ($this->blankParameterCheck($param ,"order_num")) {
            $query .= "\n   AND  A.order_num = " . $param["order_num"];
        }
        $query .= " GROUP BY A.order_num";
        return $conn->Execute($query);
    }
}
?>
