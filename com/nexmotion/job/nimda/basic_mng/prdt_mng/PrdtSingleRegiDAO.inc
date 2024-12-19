<?
include_once(INC_PATH . '/com/nexmotion/html/nimda/basic_mng/prdt_mng/RegiPopupHtml.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/basic_mng/prdt_mng/PrdtBasicRegiListHtml.inc');
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BasicMngCommonDAO.inc');

/**
 * @file PrdtBasicRegiDAO.inc
 *
 * @brief 기초관리 - 상품기초등록 - 상품기초등록 DAO
 */
class PrdtSingleRegiDAO extends BasicMngCommonDAO {

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

        $query  = "\nSELECT * FROM order_info_history ";
        $query .= "\n       WHERE  order_common_seqno = %s ";
        $query .= "\n       ORDER BY order_info_history_seqno desc ";

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
}
?>
