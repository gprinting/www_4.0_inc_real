<?
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/nimda/calcul_mng/settle/AdjustDataHTML.inc");

/**
 * @file AdjustDataDAO.inc
 *
 * @brief 정산관리 - 결산 - 조정자료 DAO
 */
class AdjustDataDAO extends NimdaCommonDAO {

    function __construct() {
    }

    /*
     * 조정 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectAdjustList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n  SELECT  A.cont";
        $query .= "\n         ,A.deal_date";
        $query .= "\n         ,A.price";
        $query .= "\n         ,B.member_name";
        $query .= "\n         ,A.input_dvs";
        $query .= "\n         ,C.name";
        $query .= "\n    FROM  adjust A";
        $query .= "\n         ,member B";
        $query .= "\n         ,empl_human_info C";
        $query .= "\n   WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n     AND  A.empl_seqno = C.empl_seqno";

        //회원일련번호가 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //회원일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n            AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {

            $query .= "\n           AND  A.deal_date >= " . $param["date_from"];

        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {

            $query .= "\n           AND  A.deal_date <= " . $param["date_to"];

        }


        $query .= "\n  ORDER BY A.regi_date DESC";
 
        //limit 조건
        if ($this->blankParameterCheck($param ,"start") 
                && $this->blankParameterCheck($param ,"end")) {
 
            $param["start"] = substr($param["start"], 1, -1);
            $param["end"] = substr($param["end"], 1, -1); 

            $query .= "\n LIMIT " . $param["start"] . ",";
            $query .= $param["end"]; 
        }

        $result = $conn->Execute($query);

        return $result;
    }


    /*
     * 조정 list Count
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function countAdjustList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n  SELECT  count(*) AS cnt";
        $query .= "\n    FROM  adjust A";
        $query .= "\n         ,member B";
        $query .= "\n         ,empl_human_info C";
        $query .= "\n   WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n     AND  A.empl_seqno = C.empl_seqno";

        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //회원일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n            AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {

            $query .= "\n           AND  A.deal_date >= " . $param["date_from"];

        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {

            $query .= "\n           AND  A.deal_date <= " . $param["date_to"];

        }

        $result = $conn->Execute($query);

        return $result;
    }

}

?>
