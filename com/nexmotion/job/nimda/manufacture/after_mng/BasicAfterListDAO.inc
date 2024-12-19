<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/after_mng/AfterMngHTML.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/manufacture/after_mng/AfterMngDOC.inc');

/**
 * @file BasicAfterListDAO.inc
 *
 * @brief 생산 - 후공정관리 - 조판별-후공정 DAO
 */
class BasicAfterListDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 생산공정관리 - 조판별 - 후공정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectBasicAfterList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        //$param = $this->parameterArrayEscape($conn, $param);

        //$dvs = substr($param["dvs"], 1, -1);

        if ($param["dvs"] == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($param["dvs"] == "SEQ") {
            $query  = "\nSELECT  A.*, B.*, MAX(B.state) as after_state ";
        }
        $query .= "\n  FROM  sheet_typset AS A ";
        $query .= "\n  INNER JOIN typset_state_history AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno ";
        if($param["state"] == "2620") {
            $query .= "\n WHERE  A.state =  '" . $param["state"] ."'";

            $query .= "\n   AND  ((B.state = '2620' AND detail LIKE '%" . $param["detail"] . "%')" . " OR (B.state = '2680' AND detail = '" . $param["detail"] . "'))";
            //$query .= "\n   AND  ((B.state = '2620' AND detail LIKE '%" . $param["detail"] . "%') OR (B.state = '2710' AND detail = '" . $param["detail"] . "'))";
            $query .= "\n   AND  A.regi_date > '" . $param["from"]. "'";
            $query .= "\n   AND  A.regi_date <= '" . $param["to"]. "'";
            $query .= "\nGROUP BY A.typset_num ";
            $query .= "\nORDER BY B.state DESC, A.regi_date DESC ";
        } else {
            $query .= "\n   AND  B.state = '2680' ";
            $query .= "\n  AND detail = '" . $param["detail"] . "' ";
            $query .= "\nGROUP BY A.typset_num ";
            $query .= "\nORDER BY B.state_modi_date DESC ";
        }
        return $conn->Execute($query);
    }

    /**
     * @brief 조판별 - 후공정공정 작업일지 보기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectBasicAfterProcessView($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\nSELECT  A.basic_after_op_seqno ";
        $query .= "\n       ,A.after_name ";
        $query .= "\n       ,A.orderer ";
        $query .= "\n       ,B.extnl_etprs_seqno ";
        $query .= "\n       ,B.extnl_brand_seqno ";
        $query .= "\n       ,A.depth1 ";
        $query .= "\n       ,A.depth2 ";
        $query .= "\n       ,A.depth3 ";
        $query .= "\n       ,A.memo ";
        $query .= "\n       ,A.amt ";
        $query .= "\n       ,A.amt_unit ";
        $query .= "\n       ,A.state ";
        $query .= "\n       ,A.flattyp_dvs ";
        $query .= "\n       ,A.typset_num ";
        $query .= "\n  FROM  basic_after_op AS A ";
        $query .= "\n       ,extnl_brand AS B ";
        $query .= "\n WHERE  A.extnl_brand_seqno = B.extnl_brand_seqno ";
        $query .= "\n   AND  A.basic_after_op_seqno = " . $param["seqno"];

        return $conn->Execute($query);
    }
}
?>
