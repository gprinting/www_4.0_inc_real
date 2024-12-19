<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/after_mng/AfterMngHTML.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/manufacture/after_mng/AfterMngDOC.inc');

/**
 * @file BasicAfterListDAO.inc
 *
 * @brief 생산 - 후공정관리 - 조판별-후공정 DAO
 */
class AfterListDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 생산공정관리 - 주문별 - 후공정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterList($conn, $param) {

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
            $query  = "\nSELECT  A.order_after_history_seqno ";
            $query .= "\n       ,B.order_num ";
            $query .= "\n       ,A.after_name ";
            $query .= "\n       ,A.depth1 ";
            $query .= "\n       ,A.depth2 ";
            $query .= "\n       ,A.depth3 ";
            $query .= "\n       ,B.amt ";
            $query .= "\n       ,B.amt_unit_dvs ";
            $query .= "\n       ,C.state ";
            $query .= "\n       ,B.work_memo ";
            $query .= "\n       ,C.state_modi_date ";
        }
        $query .= "\n  FROM order_after_history AS A ";
        $query .= "\n  INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n  LEFT JOIN order_state_history AS C ON C.order_common_seqno = A.order_common_seqno AND C.detail = A.after_name ";
        $query .= "\n   AND  C.state = 2780 ";
        $query .= "\n WHERE 1 = 1 ";

        //$query .= "\n   AND  A.order_common_seqno = D.order_common_seqno ";

        if ($this->blankParameterCheck($param ,"cate_sortcode")) {
            $val = substr($param["cate_sortcode"], 1, -1);
            $query .= "\n   AND  B.cate_sortcode LIKE '" . $val . "%' ";
        }
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  C.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param, "from")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  B.order_regi_date > " . $param["from"];
        }
        if ($this->blankParameterCheck($param, "to")) {
            $val = substr($param["date_cnd"], 1, -1);
            $query .= "\n   AND  B.order_regi_date <= " . $param["to"];
        }
        if ($this->blankParameterCheck($param ,"state")) {
            if($param["state"] == "'2780'")
                $query .= "\n   AND  C.state >= " . $param["state"];
            else {
                $query .= "\n   AND B.order_state = " . $param["state"];
            }

        } else {
            $query .= "\n AND B.order_state >= '2720'  ";
        }

        if($param["state"] != "'2720'") {
            if ($this->blankParameterCheck($param, "from")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  C.state_modi_date > " . $param["from"];
            }
            if ($this->blankParameterCheck($param, "to")) {
                $val = substr($param["date_cnd"], 1, -1);
                $query .= "\n   AND  C.state_modi_date <= " . $param["to"];
            }
        }
        if ($this->blankParameterCheck($param ,"after")) {
            $val = substr($param["after"], 1, -1);
            if($param["after"] == "'재단'") {
                $query .= "\n   AND  A.after_name LIKE '%" . $val . "%'";
            } else {
                $query .= "\n   AND  A.after_name = " . $param["after"];
            }
        }
        $query .= "\nGROUP BY B.order_num ";
        if($param["state"] != "'2780'")
            $query .= "\nORDER BY B.order_regi_date ASC ";
        else
            $query .= "\nORDER BY C.state_modi_date desc ";

        if($param["state"] != "'2720'") {
            $query .= "\nLIMIT 200 ";
        }

        return $conn->Execute($query);
    }
   
    /**
     * @brief 주문별 - 후공정공정 작업일지 보기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterProcessView($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT A.cate_sortcode";
        $query .= "\n      ,A.after_name";
        $query .= "\n          ,A.depth1";
        $query .= "\n          ,A.depth2";
        $query .= "\n          ,A.depth3";
        $query .= "\n          ,A.amt";
        $query .= "\n          ,A.amt_unit";
        $query .= "\n          ,A.memo";
        $query .= "\n          ,A.specialty_items";
        $query .= "\n          ,A.dlvrboard";
        $query .= "\n          ,A.state";
        $query .= "\n          ,A.orderer";
        $query .= "\n          ,A.order_common_seqno";
        $query .= "\n          ,A.order_detail_dvs_num";
        $query .= "\n          ,A.specialty_items";
        $query .= "\n          ,B.order_num";
        $query .= "\n          ,B.oper_sys";
        $query .= "\n          ,B.order_detail";
        $query .= "\n          ,C.extnl_brand_seqno";
        $query .= "\n          ,C.extnl_etprs_seqno"; 
        $query .= "\n  FROM after_op AS A";
        $query .= "\n      ,order_common AS B";
        $query .= "\n      ,extnl_brand AS C";
        $query .= "\n WHERE A.after_op_seqno = " . $param["seqno"];
        $query .= "\n   AND A.order_common_seqno = B.order_common_seqno";
        $query .= "\n   AND A.extnl_brand_seqno = C.extnl_brand_seqno";

        return $conn->Execute($query);
    }

    //order_common order_state 변경
	function updateOrderCommonState($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common set order_state = " . $param['state'];
		$query .= "\n  WHERE order_common_seqno = ". $param['order_common_seqno'];

        return $conn->Execute($query);
	}

    function selectOrderAfterHistorySeqno($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT B.order_after_history_seqno FROM order_common AS A ";
        $query .= "\n INNER JOIN order_after_history AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n  WHERE A.order_num = %s AND B.after_name = %s ";

        $query  = sprintf($query, $param["ordernum"], $param["after_name"]);
        return $conn->Execute($query);
    }

    //order_common order_state 변경
    function selectCompleteAfter($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT COUNT(*) cnt FROM order_after_history AS A ";
        $query .= "\n  INNER JOIN order_state_history AS C ON C.order_common_seqno = A.order_common_seqno AND C.detail = A.after_name ";
        $query .= "\n  WHERE A.order_common_seqno = ". $param['order_common_seqno'];
        $query .= "\n  AND A.before_cutting_yn = 'N' AND (A.basic_yn = 'N' OR A.after_name='봉투가공')";

        return $conn->Execute($query);
    }

    function selectAllAfter($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT COUNT(*) cnt FROM order_after_history AS A ";
        $query .= "\n  WHERE A.order_common_seqno = ". $param['order_common_seqno'];
        $query .= "\n  AND A.before_cutting_yn = 'N' AND (A.basic_yn = 'N' OR A.after_name='봉투가공')";

        return $conn->Execute($query);
    }
}
?>
