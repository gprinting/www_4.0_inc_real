<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/item_mng/PaperOrdPrintHtml.inc');

/**
 * @file PaperOrdPrintDAO.inc
 *
 * @brief 생산 - 자재관리 - 종이발발주서인쇄 DAO
 */
class PaperOrdPrintDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 종이발주인쇄 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaperOpMngPrintList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  A.paper_op_seqno ";
        $query .= "\n       ,A.op_num ";
        $query .= "\n       ,A.op_degree ";
        $query .= "\n       ,A.op_date ";
        $query .= "\n       ,A.name ";
        $query .= "\n       ,A.dvs ";
        $query .= "\n       ,A.color ";
        $query .= "\n       ,A.basisweight ";
        $query .= "\n       ,A.stor_subpaper ";
        $query .= "\n       ,A.stor_size ";
        $query .= "\n       ,A.grain ";
        $query .= "\n       ,SUM(A.amt) amt ";
        $query .= "\n       ,A.amt_unit ";
        $query .= "\n       ,A.memo ";
        $query .= "\n       ,A.storplace";
        $query .= "\n       ,B.etprs_name";
        $query .= "\n  FROM  paper_op AS A ";
        $query .= "\n       INNER JOIN extnl_etprs AS B ON A.extnl_etprs_seqno = B.extnl_etprs_seqno ";
        $query .= "\n WHERE  1 = 1 ";
        $query .= "\n   AND  A.state >= " . $param["state"];
        if ($this->blankParameterCheck($param ,"extnl_etprs_seqno")) {
            $query .= "\n   AND  C.extnl_etprs_seqno = " . $param["extnl_etprs_seqno"];
        }
        if ($this->blankParameterCheck($param ,"from")) {
            $query .= "\n   AND  A.op_date >= " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $query .= "\n   AND  A.op_date <= " . $param["to"];
        }

        $query .= "\n   group by A.op_degree, A.name, A.stor_size, A.storplace, A.grain ";
        $query .= "\n   order by A.op_degree, A.storplace ";
        return $conn->Execute($query);
    }
}
