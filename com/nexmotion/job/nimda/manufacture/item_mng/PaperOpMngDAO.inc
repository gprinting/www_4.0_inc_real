<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/item_mng/PaperOpMngHtml.inc');

/**
 * @file PaperOpMngDAO.inc
 *
 * @brief 생산 - 자재관리 - 종이발주관리 DAO
 */
class PaperOpMngDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }
   
    /**
     * @brief 조판등록 - 종이 등록업체리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaperInfoList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT A.name AS paper_name ";
        $query .= "\n       ,A.dvs ";
        $query .= "\n       ,A.color ";
        $query .= "\n       ,A.basisweight ";
        $query .= "\n       ,A.basisweight_unit ";
        $query .= "\n       ,A.affil ";
        $query .= "\n       ,A.size ";
        $query .= "\n       ,A.prdt_paper_seqno ";
        $query .= "\n  FROM  prdt_paper AS A ";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param ,"name")) {
            $query .= "\n   AND  A.name = " . $param["name"];
        }
        if ($this->blankParameterCheck($param ,"dvs")) {
            $query .= "\n   AND  A.dvs = " . $param["dvs"];
        }
        if ($this->blankParameterCheck($param ,"color")) {
            $query .= "\n   AND  A.color = " . $param["color"];
        }
        if ($this->blankParameterCheck($param ,"basisweight") && 
                $this->blankParameterCheck($param ,"basisweight_unit")) {
            $query .= "\n   AND  A.basisweight = " . $param["basisweight"];
            $query .= "\n   AND  A.basisweight_unit = " . $param["basisweight_unit"];
        }
        if ($this->blankParameterCheck($param ,"sorting")) {
            $sorting = substr($param["sorting"], 1, -1);
            $query .= "\n ORDER BY " . $sorting;

            if ($this->blankParameterCheck($param ,"sorting_type")) {
                $sorting_type = substr($param["sorting_type"], 1, -1);
                $query .= " " . $sorting_type;
            }
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 종이 등록업체리스트 -적용
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaperInfoApply($conn, $param) {
 
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\nSELECT  ";
        $query .= "\n       A.name ";
        $query .= "\n       ,A.dvs ";
        $query .= "\n       ,A.color ";
        $query .= "\n       ,A.basisweight ";
        $query .= "\n       ,A.basisweight_unit ";
        $query .= "\n       ,A.affil ";
        $query .= "\n       ,A.size ";
        $query .= "\n       ,A.prdt_paper_seqno ";
        $query .= "\n  FROM  prdt_paper AS A ";
        $query .= "\n WHERE  1 = 1 ";

        //일련번호 
        if ($this->blankParameterCheck($param ,"prdt_paper_seqno")) {
            $query .= "\n   AND  A.prdt_paper_seqno = " . $param["prdt_paper_seqno"];
        }
        return $conn->Execute($query);
    }

    /**
     * @brief 종이 발주
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaperOpLastNum($conn) {
        $today = date("Y-m-d");

        $query  = "\n   SELECT op_num";
        $query .= "\n     FROM paper_op";
        $query .= "\n    WHERE '%s 00:00:00' <= regi_date";
        $query .= "\n      AND regi_date <= '%s 23:59:59'";
        $query .= "\n ORDER BY paper_op_seqno DESC";
        $query .= "\n    LIMIT 1";

        $query  = sprintf($query, $today, $today);

        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $last_num = 1;
        } else {
            $last_num = intval(substr($rs->fields["op_num"], 6)) + 1;
        }

        return $last_num;
    }

    function selectSize($conn, $param) {
        $query  = "\n   SELECT width, height";
        $query .= "\n     FROM sheet_typset";
        $query .= "\n    WHERE typset_num = '%s' ";

        $query  = sprintf($query, $param['typset_num']);

        $rs = $conn->Execute($query);
        return $rs;
    }
}
?>
