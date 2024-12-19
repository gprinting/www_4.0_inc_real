<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/MemberCommonDAO.inc');

class MemberFindIdDAO extends MemberCommonDAO {
    function __construct() {
    }
 
    /**
     * @brief 아이디 찾기
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectFindId($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  member_seqno ";
        $query .= "\n  FROM  member ";
        $query .= "\n WHERE  member_name = " . $param["member_name"];
        $query .= "\n   AND  mail        = " . $param["mail"];
        $query .= "\n   AND  join_path   = 'Normal'";

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    /**
     * @brief 임시비밀번호로 업데이트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberTempPw($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  passwd = PASSWORD(%s) ";
        $query .= "\n           ,final_modi_date = now()";
        $query .= "\n     WHERE  member_seqno = %s ";
        $query .= "\n       AND  join_path = %s ";

        $query = sprintf($query, $param["passwd"]
                               , $param["member_seqno"]
                               , $param["join_path"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }
    /**
     * @brief 아이디 찾기 결과 정보 (180221 이거 안 쓰는것 같은데 확인 필요함)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectFindIdInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $search_cnd = substr($param["search_cnd"], 1, -1);

        $query  = "\nSELECT  member_name ";
        $query .= "\n       ,member_id ";
        $query .= "\n       ,mail ";
        $query .= "\n       ,cell_num ";
        $query .= "\n       ,first_join_date ";
        $query .= "\n  FROM  member ";
        $query .= "\n WHERE  1 = 1 ";

        //일련번호 검색
        if ($this->blankParameterCheck($param ,"seqno")) {
            $query .= "\n   AND  member_seqno = $param[seqno]";
        }
 
        return $conn->Execute($query);
    }
}
?>
