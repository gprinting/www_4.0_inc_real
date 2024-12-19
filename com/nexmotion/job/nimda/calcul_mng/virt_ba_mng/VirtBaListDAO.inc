<?
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/nimda/calcul_mng/virt_ba_mng/VirtBaListHTML.inc");

/**
 * @file VirtBaListDAO.inc
 *
 * @brief 정산관리 - 가상계좌관리 - 가상계좌리스트 DAO
 */
class VirtBaListDAO extends NimdaCommonDAO {

    function __construct() {
    }

    /*
     * 가상계좌 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectVirtBaList($conn, $param) {
        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n  SELECT  A.ba_num";
        $query .= "\n         ,A.use_yn";
        $query .= "\n         ,A.bank_name";
        $query .= "\n         ,A.member_seqno";
        $query .= "\n         ,A.virt_ba_admin_seqno";
        $query .= "\n         ,B.member_name";
        //$query .= "\n    FROM  virt_ba_admin A ";
        $query .= "\n    FROM  virt_ba_admin A ";
        $query .= "\n    LEFT OUTER JOIN  member B ";
        $query .= "\n      ON  A.member_seqno = B.member_seqno ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }
 
        //계좌번호가 있을때
        if ($this->blankParameterCheck($param ,"ba_num")) {

            $query .= "\n            AND A.ba_num LIKE '%";
            $query .= substr($param["ba_num"], 1,-1) . "%'";
        }

        //예금주가 있을때
        if ($this->blankParameterCheck($param, "member_name")) {

            $query .= "\n            AND  B.member_name LIKE '%";
            $query .= substr($param["member_name"], 1,-1) . "%'";

       }

        //상태가 있을때
        if ($this->blankParameterCheck($param, "state")) {

            $query .= "\n            AND  A.state = " . $param["state"];

       }

        $query .= "\n  ORDER BY A.virt_ba_admin_seqno ASC";
 
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
     * 가상계좌 list Select
     * $conn : DB Connection
     * return : resultSet
     */
    function selectVirtBaChangeList($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n         SELECT A.depo_name";
        $query .= "\n         ,A.bank_before";
        $query .= "\n         ,A.bank_aft";
        $query .= "\n         ,A.prog_state"; // 허가일
        $query .= "\n         ,A.virt_ba_change_history_seqno ";
        $query .= "\n         ,B.member_name";

        $query .= "\n    FROM  virt_ba_change_history A ";
        $query .= "\n    INNER JOIN  member B ON  A.member_seqno = B.member_seqno ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //상태가 있을때
        if ($this->blankParameterCheck($param, "state")) {

            $query .= "\n            AND  A.prog_state = " . $param["state"];

        }

        $query .= "\n  ORDER BY A.virt_ba_change_history_seqno ASC";

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

    function selectVirtBaRefundList($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n         SELECT A.req_date";
        $query .= "\n         ,A.req_price";
        $query .= "\n         ,A.member_refund_history_seqno ";
        $query .= "\n         ,A.refund_yn";
        $query .= "\n         ,B.member_name";
        $query .= "\n         ,B.refund_name";
        $query .= "\n         ,B.refund_ba_num";
        $query .= "\n         ,B.refund_bank_name";

        $query .= "\n    FROM  member_refund_history A ";
        $query .= "\n    INNER JOIN  member B ON  A.member_seqno = B.member_seqno ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //상태가 있을때
        if ($this->blankParameterCheck($param, "state")) {

            $query .= "\n            AND  A.refund_yn = " . $param["state"];

        }

        $query .= "\n  ORDER BY A.member_refund_history_seqno ASC";

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
     * 가상계좌 list Select
     * $conn : DB Connection
     * return : resultSet
     */
    function selectVirtBaChangeDetail($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n         SELECT A.depo_name";
        $query .= "\n         ,A.bank_aft";
        $query .= "\n         ,A.prog_state"; // 허가일
        $query .= "\n         ,A.virt_ba_change_history_seqno ";
        $query .= "\n         ,A.member_seqno ";
        $query .= "\n         ,C.ba_num ";
        $query .= "\n         ,C.bank_name ";

        $query .= "\n    FROM  virt_ba_change_history A ";
        $query .= "\n    LEFT JOIN virt_ba_admin C ON A.member_seqno = C.member_seqno AND C.use_yn = 'Y' ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"seq")) {

            $query .= "\n            AND  A.virt_ba_change_history_seqno = ";
            $query .= $param["seq"];

        }
        $result = $conn->Execute($query);

        return $result;
    }

    function selectVirtBaRefundDetail($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n         SELECT *";

        $query .= "\n    FROM  member_refund_history A ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"seq")) {

            $query .= "\n            AND  A.member_refund_history_seqno = ";
            $query .= $param["seq"];

        }
        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 가상계좌 list Count 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function countVirtBaList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n  SELECT  count(*) AS cnt";
        $query .= "\n    FROM  virt_ba_admin A ";
        $query .= "\n    LEFT OUTER JOIN  member B ";
        $query .= "\n      ON  A.member_seqno = B.member_seqno ";
        $query .= "\n   WHERE  1=1";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }
 
        //계좌번호가 있을때
        if ($this->blankParameterCheck($param ,"ba_num")) {

            $query .= "\n       AND ba_num LIKE '%";
            $query .= substr($param["ba_num"], 1,-1) . "%'";
        }

        //예금주가 있을때
        if ($this->blankParameterCheck($param, "member_seqno")) {

            $query .= "\n            AND  B.member_seqno = ";
            $query .= $param["member_seqno"];

       }

        //상태가 있을때
        if ($this->blankParameterCheck($param, "state")) {

            $query .= "\n            AND  A.state = " . $param["state"];

       }

         $result = $conn->Execute($query);

        return $result;
    }


    /*
     * 가상계좌번호 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectBaNumList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  SELECT  ba_num";
        $query .= "\n    FROM  virt_ba_admin";
        $query .= "\n   WHERE  cpn_admin_seqno = " . $param["cpn_admin_seqno"];

        //가상계좌번호가 있을때
        if ($this->blankParameterCheck($param, "search")) {
            $query .= "\n     AND ba_num LIKE '%";
            $query .= substr($param["search"], 1,-1) . "%'";
        }

        $result = $conn->Execute($query);

        return $result;

    }

    /*
     * 회원이름 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectMemberSeqList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  SELECT  member_name";
        $query .= "\n         ,member_seqno";
        $query .= "\n    FROM  member";
        $query .= "\n   WHERE  cpn_admin_seqno = " . $param["cpn_admin_seqno"];

        //회원이름이 있을때
        if ($this->blankParameterCheck($param, "search")) {
            $query .= "\n     AND member_name LIKE '%";
            $query .= substr($param["search"], 1,-1) . "%'";
        }

        $result = $conn->Execute($query);

        return $result;


    }

    function selectMemberByBaNum($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);
        $query = "\n  SELECT  member_seqno";
        $query .= "\n    FROM  virt_ba_admin";
        //$query .= "\n    FROM  virt_ba_admin";
        $query .= "\n   WHERE  ba_num = %s AND bank_name = %s ";


        $query = sprintf($query, $param["account_no"],$param["bank_nm"]);
        return $conn->Execute($query);
    }

    function selectDealNum($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);
        $query = "\n  SELECT  COUNT(*) cnt";
        $query .= "\n    FROM  member_pay_history ";
        $query .= "\n   WHERE  deal_num = %s ";


        $query = sprintf($query, $param["deal_num"]);
        return $conn->Execute($query);
    }

    function insertVirtAmount($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n  INSERT INTO member_pay_history ";
        $query .= "\n  (member_seqno, deal_date, dvs, sell_price, depo_price, deal_num, pay_year, pay_mon, prepay_use_yn, cont, depo_bank_code, depo_bank_name, input_typ)";
        $query .= "\n  VALUES ";
        $query .= "\n  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) ";

        $query = sprintf($query
            ,$param["member_seqno"]
            ,$param["deal_date"]
            ,$param["dvs"]
            ,$param["sell_price"]
            ,$param["depo_price"]
            ,$param["deal_num"]
            ,$param["pay_year"]
            ,$param["pay_mon"]
            ,$param["prepay_use_yn"]
            ,$param["cont"]
            ,$param["depo_bank_code"]
            ,$param["depo_bank_name"]
            ,"100");

        $conn->Execute($query);
        return $query;
    }

    /*
     * 가상계좌 회원 맵핑 Update
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function updateMemberVirtBa($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  UPDATE  virt_ba_admin";
        $query .= "\n     SET  state = 'N'";
        $query .= "\n         ,member_seqno = NULL";
        $query .= "\n   WHERE  virt_ba_admin_seqno = " . $param["virt_seqno"];

        $result = $conn->Execute($query);

        if ($result === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /*
     * 가상계좌 정보 수정 Update
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function updateVirtBaInfo($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  UPDATE  virt_ba_admin";
        $query .= "\n     SET cpn_admin_seqno = " . $param["cpn_admin_seqno"];

        //은행이 있을때
        if ($this->blankParameterCheck($param, "bank_name")) {

            $query .= "\n        ,bank_name = " . $param["bank_name"];

       }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param, "member_seqno")) {

            $query .= "\n        ,member_seqno = " . $param["member_seqno"];
            $query .= "\n        ,state = 'Y'";

       } else {

            $query .= "\n        ,member_seqno = NULL";
            $query .= "\n        ,state = 'N'";

        }

        $query .= "\n  WHERE  virt_ba_admin_seqno = " . $param["virt_seqno"];

        $result = $conn->Execute($query);

        if ($result === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /*
     * 회원과 맵핑된 가상계좌 Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectMemberVirtBa($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  SELECT  virt_ba_admin_seqno";
        $query .= "\n    FROM  virt_ba_admin";
        $query .= "\n   WHERE  member_seqno = " . $param["member_seqno"];

        $result = $conn->Execute($query);

        return $result;
    }



}

?>
