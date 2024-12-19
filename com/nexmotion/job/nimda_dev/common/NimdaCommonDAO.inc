<?
include_once(INC_PATH . '/common_dao/CommonDAO.inc');

class NimdaCommonDAO extends CommonDAO {
    /**
     * @brief 로그인 한 직원정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 직원 검색조건
     *
     * @return 검색결과
     */
    public function selectEmplLogin($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [NimdaCommonDAO.inc] 로그인 한 직원정보 검색 */";
        $query .= "\n         A.empl_seqno";
        $query .= "\n        ,A.empl_num";
        $query .= "\n        ,A.admin_auth";
        $query .= "\n        ,A.able_top_menu";
        $query .= "\n        ,A.able_btm_menu";
        $query .= "\n        ,B.name";
        $query .= "\n   FROM  empl AS A";
        $query .= "\n        ,empl_human_info AS B";
        $query .= "\n  WHERE  A.empl_id = %s";
        $query .= "\n    AND  A.passwd = password(%s)";

        $query  = sprintf($query, $param["empl_id"]
                                , $param["passwd"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 직원의 메뉴코드 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 직원 검색조건
     *
     * @return 검색결과
     */
    public function selectEmplMenu($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [NimdaCommonDAO.inc] 직원의 메뉴코드 검색 */";
        $query .= "\n         able_top_menu";
        $query .= "\n        ,able_btm_menu";
        $query .= "\n   FROM  empl";
        $query .= "\n  WHERE  empl_seqno = %s";

        $query  = sprintf($query, $conn->param($param["empl_seqno"]));

        $rs = $conn->Execute($query, [$param["empl_seqno"]]);

        return $rs->fields;
    }

    /**
     * @brief 회원의 접근 가능한 메뉴코드 검색
     *
     * @param $conn      = db 커넥션
     * @param $cacheFlag = 캐쉬여부(true -> 사용, flag -> 캐쉬 flush)
     *
     * @return 검색결과
     */
    public function selectMemberGradePolicy($conn, $cache_flag = true) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [NimdaCommonDAO.inc] 회원 등급정책 검색 */";
        $query .= "\n         erp_grade_name";
        $query .= "\n        ,grade";
        $query .= "\n   FROM  member_grade_policy";

        if ($cache_flag) {
            $rs = $conn->CacheExecute(3600, $query);
            return $rs;
        } else {
            $conn->CacheFlush($query);
            return null;
        }
    }
}
