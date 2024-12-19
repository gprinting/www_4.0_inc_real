<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');

/**
 * @file BusinessCommonDAO.inc
 *
 * @brief 영업 공통DAO
 */
class BusinessCommonDAO extends NimdaCommonDAO {
    function __construct() {
    }

    /**
     * @brief 주문 테이블 마지막 seqno 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 마지막 seqno
     */
    function selectLastOrderSeqno($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  order_common_seqno AS seqno";
        $query .= "\n   FROM  order_common";
        $query .= "\n  ORDER BY order_common_seqno DESC";
        $query .= "\n  LIMIT  1";

        $rs = $conn->Execute($query);

        return $rs->fields["seqno"];
    }
    /**
     * @brief 접수(영업)팀 option html 생성
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return option html, 첫 번째 부서코드
     */
    function selectReceiptDeparInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $rs = $this->selectReceiptDepar($conn, $param);

        $depar_code = $rs->fields["depar_code"];

        $temp = array();
        $temp["flag"] = 'Y';
        $temp["def"] = "전체";
        $temp["dev_val"] = '';
        $temp["val"] = "depar_code";
        $temp["dvs"] = "depar_name";

        return array(
            "depar_code" => $depar_code,
            "html"       => makeSelectOptionHtml($rs, $temp)
        );
    }

    /**
     * @brief 접수(영업)팀에 속한 직원 option html 생성
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return option html
     */
    function selectEmplHtml($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT  /* 영업.주문관리 - 접수팀에 속한 직원 검색 */";
        $query .= "\n           empl_seqno";
        $query .= "\n          ,name";
        $query .= "\n     FROM  empl";
        $query .= "\n    WHERE  depar_code = %s";
        $query .= "\n ORDER BY  posi_code";

        $query  = sprintf($query, $param["depar_code"]);

        $rs = $conn->Execute($query);

        $temp = array();
        $temp["flag"] = 'Y';
        $temp["def"] = "전체";
        $temp["def_val"] = '';
        $temp["val"] = "empl_seqno";
        $temp["dvs"] = "name";

        return makeSelectOptionHtml($rs, $temp);
    }

}
?>
