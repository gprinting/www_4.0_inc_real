<?
include_once(INC_PATH . '/com/nexmotion/html/nimda/basic_mng/prdt_mng/RegiPopupHtml.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/basic_mng/prdt_mng/PrdtBasicRegiListHtml.inc');
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BasicMngCommonDAO.inc');

/**
 * @file PrdtBasicRegiDAO.inc
 *
 * @brief 기초관리 - 상품기초등록 - 상품기초등록 DAO
 */
class PrdtBasicRegiDAO extends BasicMngCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 상품 분류 가져옴 
     * table prdt_sort 
     */
    function selectPrdtSort($conn, $param) {

        //커넥션 체크
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        //카테고리 분류코드 빈값 체크
        if (!$this->blankParameterCheck($param ,"prdt_dvs")) {
            return false;
        }

        $query  = "\n SELECT sort ";         
        $query .= "\n       ,prdt_sort_seqno ";         
        $query .= "\n   FROM prdt_sort ";            
        $query .= "\n  WHERE prdt_dvs = " . $param["prdt_dvs"];

        return $conn->Execute($query);
    }

    function selectOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT * FROM order_info_history AS A ";
        $query .= "\n       LEFT JOIN empl AS B ON A.empl_id = B.empl_id";
        $query .= "\n       LEFT JOIN empl_human_info AS C ON B.empl_seqno = C.empl_seqno";
        $query .= "\n       WHERE  A.order_common_seqno = %s ";
        $query .= "\n       ORDER BY A.order_info_history_seqno desc ";

        $query  = sprintf($query,  $param["seq"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectPrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT pay_price FROM order_common ";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query,  $param["seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectInvoNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n       SELECT A.invo_num FROM order_dlvr AS A ";
        $query .= "\n       INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n       WHERE  A.order_common_seqno = %s AND A.tsrs_dvs = '수신' ";

        $query  = sprintf($query,  $param["seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }
}
?>
