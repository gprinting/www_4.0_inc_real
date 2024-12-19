<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/cutting_mng/CuttingMngHtml.inc');
//include_once(INC_PATH . '/com/nexmotion/doc/nimda/manufacture/cutting_mng/CuttingMngDOC.inc');

/**
 * @file PrintListDAO.inc
 *
 * @brief 생산 - 재단관리 - 재단리스트 DAO
 */
class CuttingListDAO extends ManufactureCommonDAO {

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
    function selectCuttingList($conn, $param) {

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
            if($param["state"] == "'2480'")
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
     * @brief 생산공정관리 - 인쇄
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCuttingProcess($conn, $param) {

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
        $query .= "\n  LEFT JOIN  typset_state_history AS E ON E.sheet_typset_seqno = A.sheet_typset_seqno AND E.state = '2480' ";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $query .= "\n   AND  D.preset_cate = " . $param["preset_cate"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  B.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2480'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }
        if($param["state"] == "'2480'") {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date <= " . $param["to"];
            }
        } else {
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
        if($param["state"] == "'2480'") {
            $query .= "\nORDER BY E.state_modi_date DESC ";
            $query .= "\nLIMIT 200 ";
        } else {
            //$query .= "\nORDER BY A.regi_date DESC ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 생산공정관리 - 인쇄
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCuttingProcess2($conn, $param) {

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
            $query  = "\nSELECT  * ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  JOIN  order_typset AS B ON A.typset_num  = B.typset_num ";
        $query .= "\n  JOIN  order_common as C on B.order_num  = C.order_num ";
        $query .= "\n  JOIN  member as D on C.member_seqno  = D.member_seqno ";
        $query .= "\n LEFT JOIN  (SELECT COUNT(*) AS num, order_common_seqno  FROM order_after_history where after_name not in ('귀도리') GROUP BY order_common_seqno ) AS E ON  C.order_common_seqno = E.order_common_seqno ";

        /*$query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  typset_state_history AS E ON E.sheet_typset_seqno = A.sheet_typset_seqno AND E.state = '2480' "; */

        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $query .= "\n   AND  D.preset_cate = " . $param["preset_cate"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  B.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2480'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }

         $query .= "\n   AND  C.order_state > 2380 ";

       if($param["after"] == "'y'") {
           $query .= "\n   AND  E.num > 1 ";
       }else{
          $query .= "\n   AND  E.num = 1 ";
       }

      /*  if($param["state"] == "'2480'") {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date <= " . $param["to"];
            }
        } else {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date <= " . $param["to"];
            }
        } */
      //  $query .= "\nGROUP BY A.typset_num ";
      /*  if($param["state"] == "'2480'") {
            $query .= "\nORDER BY E.state_modi_date DESC ";
            $query .= "\nLIMIT 200 ";
        } else {
            //$query .= "\nORDER BY A.regi_date DESC ";
        } */
        //echo $query;
        return $conn->Execute($query);
    }

    /**
     * @brief 생산공정관리 - 인쇄
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCuttingProcess3($conn, $param) {

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
            $query  = "\nSELECT  C.order_num ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  JOIN  order_typset AS B ON A.typset_num  = B.typset_num ";
        $query .= "\n  JOIN  order_common as C on B.order_num  = C.order_num ";
        $query .= "\n  JOIN  member as D on C.member_seqno  = D.member_seqno ";

        /*$query .= "\n  LEFT JOIN  sheet_typset_label_file AS D ON A.sheet_typset_seqno = D.sheet_typset_seqno ";
        $query .= "\n  LEFT JOIN  typset_state_history AS E ON E.sheet_typset_seqno = A.sheet_typset_seqno AND E.state = '2480' "; */

        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $query .= "\n   AND  D.preset_cate = " . $param["preset_cate"];
        }
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  B.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2480'")
                $query .= "\n   AND  A.state >= " . $param["state"];
            else
                $query .= "\n   AND  A.state = " . $param["state"];
        }

       if($param["after"] == "'y'") {
           $query .= "\n   AND  C.add_after_price != 0 group by C.order_num ";
       }else{
          $query .= "\n   AND  C.add_after_price = 0  group by C.order_num";
       }
      /*  if($param["state"] == "'2480'") {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  E.state_modi_date <= " . $param["to"];
            }
        } else {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  A.regi_date <= " . $param["to"];
            }
        } */
      //  $query .= "\nGROUP BY A.typset_num ";
      /*  if($param["state"] == "'2480'") {
            $query .= "\nORDER BY E.state_modi_date DESC ";
            $query .= "\nLIMIT 200 ";
        } else {
            //$query .= "\nORDER BY A.regi_date DESC ";
        } */
        //echo $query;
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
    function selectCuttingProcessView($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  A.name AS print_name";
        $query .= "\n       ,A.print_op_seqno ";
        $query .= "\n       ,A.typset_num ";
        $query .= "\n       ,A.orderer ";
        $query .= "\n       ,B.extnl_brand_seqno ";
        $query .= "\n       ,B.extnl_etprs_seqno ";
        $query .= "\n       ,A.typ ";
        $query .= "\n       ,A.typ_detail ";
        $query .= "\n       ,A.size ";
        $query .= "\n       ,A.affil ";
        $query .= "\n       ,A.subpaper ";
        $query .= "\n       ,A.amt ";
        $query .= "\n       ,A.amt_unit ";
        $query .= "\n       ,A.beforeside_tmpt ";
        $query .= "\n       ,A.beforeside_spc_tmpt ";
        $query .= "\n       ,A.aftside_tmpt ";
        $query .= "\n       ,A.aftside_spc_tmpt ";
        $query .= "\n       ,A.tot_tmpt ";
        $query .= "\n       ,A.memo ";
        $query .= "\n       ,A.state ";
        $query .= "\n       ,A.flattyp_dvs ";
        $query .= "\n       ,A.paper_stor_yn ";
        $query .= "\n  FROM  print_op AS A ";
        $query .= "\n       ,extnl_brand AS B ";
        $query .= "\n WHERE  A.extnl_brand_seqno = B.extnl_brand_seqno ";

        //일련번호
        if ($this->blankParameterCheck($param ,"seqno")) {
            $query .= "\n   AND  A.print_op_seqno = " . $param["seqno"];
        }

        //조판번호
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $query .= "\n   AND  A.typset_num = " . $param["typset_num"];
        }

        return $conn->Execute($query);
    }
}
?>
