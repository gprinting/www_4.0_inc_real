<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/print_mng/PrintMngHtml.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/manufacture/print_mng/PrintMngDOC.inc');

/**
 * @file PrintListDAO.inc
 *
 * @brief 생산 - 인쇄관리 - 인쇄리스트 DAO
 */
class PrintListDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 생산공정관리 - 인쇄
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrintList($conn, $param) {

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
            $query .= "\n       ,CONCAT(C.file_path, C.save_file_name) as typset_pdf  ";
            $query .= "\n       ,CONCAT(D.file_path, D.save_file_name) as typset_label ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  INNER JOIN  state_admin AS B ON A.state = B.state_code ";
        $query .= "\n  LEFT JOIN  sheet_typset_file AS C ON A.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $query .= "\n   AND  D.preset_cate = " . $param["preset_cate"];
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  B.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2420'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A." . $val ." > " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  A." . $val. " <= " . $param["to"];
        }

        $query .= "\nORDER BY A.regi_date DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 인쇄공정 작업일지 보기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrintProcessView($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  ";
        $query .= "\n       A.print_title ";
        $query .= "\n       ,A.paper_name ";
        $query .= "\n       ,A.print_amt ";
        $query .= "\n       ,A.size ";
        $query .= "\n       ,A.memo ";
        $query .= "\n       ,A.beforeside_tmpt ";
        $query .= "\n       ,A.aftside_tmpt ";
        $query .= "\n       FROM  sheet_typset AS A  ";
        $query .= "\n       WHERE  A.typset_num = " . $param["typset_num"];

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
    function selectPrintProcess($conn, $param) {
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
        $query .= "\n  LEFT JOIN  typset_state_history AS E ON E.sheet_typset_seqno = A.sheet_typset_seqno AND (E.state = '2420' OR E.state = '2620') ";
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
            if($param["state"] == "'2380'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }

        if($param["state"] == "'2320'" || $param["state"] == "'2380'") {
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
        if($param["state"] == "'2380'") {

            $query .= "\nORDER BY E.state_modi_date DESC ";
            $query .= "\nLIMIT 200 ";
            //$query .= "\nLIMIT 30 ";
        }

        return $conn->Execute($query);
    }
}
?>
