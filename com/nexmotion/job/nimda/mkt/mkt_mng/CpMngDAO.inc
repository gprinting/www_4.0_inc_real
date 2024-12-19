<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/MktCommonDAO.inc');
include_once(INC_PATH . "/com/nexmotion/html/nimda/mkt/mkt_mng/CpMngHTML.inc");

class CpMngDAO extends MktCommonDAO {

    function __construct() {
    }

    /*
     * 쿠폰 list select 
     * $conn : db connection
     * $param["cpn_seqno"] : 회사 관리 일련번호
     * return : resultset 
     */ 
    function selectCpList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT   A.name";
        $query .= "\n            ,A.release_date";
        $query .= "\n            ,A.expired_date";
        $query .= "\n            ,A.detail";
        $query .= "\n            ,A.discount_rate";
        $query .= "\n            ,A.coupon_seqno";
        $query .= "\n            ,A.coupon_kind";
        $query .= "\n            ,A.categories";
        $query .= "\n            ,A.is_released";
        $query .= "\n            ,A.coupon_number";
        $query .= "\n            ,B.sell_site";
        $query .= "\n      FROM   coupon A";
        $query .= "\n            ,cpn_admin B";
        $query .= "\n     WHERE  A.cpn_admin_seqno = B.cpn_admin_seqno";

        //판매사이트가 있을때
        if ($this->blankParameterCheck($param ,"cpn_seqno")) {

            $query .= "\n       AND  B.cpn_admin_seqno =" . $param["cpn_seqno"];
        }

        //쿠폰 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"coupon_seqno")) {

            $query .= "\n       AND  A.coupon_seqno =" . $param["coupon_seqno"];
        }

        $result = $conn->Execute($query);

        return $result;

    }

    /*
     * 회원 정보  list select 
     * $conn : db connection
     * $param["cpn_seqno"] : 회사 관리 일련번호
     * return : resultset 
     */ 
    function selectMemberInfoList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    SELECT   office_nick";
        $query .= "\n            ,grade";
        $query .= "\n            ,member_typ";
        $query .= "\n            ,cell_num";
        //$query .= "\n            ,office_member_mng";
        $query .= "\n            ,member_seqno";
        $query .= "\n      FROM   member";
        //그룹 아이디가 없을때
        $query .= "\n     WHERE    member_dvs != '기업개인'";
        $query .= "\n       AND    withdraw_dvs = '1'";

        //판매사이트가 있을때
        if ($this->blankParameterCheck($param ,"cpn_seqno")) {

            $query .= "\n       AND  cpn_admin_seqno =" . $param["cpn_seqno"];
        }

        //회원명 검색 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n       AND  member_seqno =" . $param["member_seqno"];
        }

        //팀 구분 검색 있을때
        if ($this->blankParameterCheck($param ,"depar_dvs")) {

            $query .= "\n       AND  ( biz_resp  =" . $param["depar_dvs"];
            $query .= "\n       OR     release_resp  =" . $param["depar_dvs"];
            $query .= "\n       OR     dlvr_resp  =" . $param["depar_dvs"] . ")";

        }

        //등급 검색 있을때
        if ($this->blankParameterCheck($param ,"grade")) {

            $query .= "\n       AND  grade =" . $param["grade"];
        }
 
        //회원 구분 있을때
        if ($this->blankParameterCheck($param ,"member_typ")) {

            $query .= "\n       AND  member_typ =" . $param["member_typ"];

        }

        $result = $conn->Execute($query);

        return $result;
 
    }

    function selectMembersForCouponRelease($conn) {

        if (!$this->connectioncheck($conn)) return false;
        $query  = "\n    SELECT   A.member_seqno";
        $query .= "\n      FROM   member A";

        $result = $conn->Execute($query);

        return $result;
    }

    function insertCoupon($conn, $param) {
        if (!$this->connectioncheck($conn)) return false;
        $query  = "\n    INSERT INTO coupon_release_history ";
        $query .= "\n      (coupon_seqno, member_seqno) VALUES (%s, %s)";

        $query  = sprintf($query, $param["coupon_seqno"], $param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;
    }

    function updateCouponReleaseComplete($conn, $param) {
        if (!$this->connectioncheck($conn)) return false;
        $query  = "\n    UPDATE coupon ";
        $query .= "\n    SET is_released = 'Y' ";
        $query .= "\n      WHERE coupon_seqno = %s ";

        $query  = sprintf($query, $param["coupon_seqno"]);

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 쿠폰 발급  list select 
     * $conn : db connection
     * $param["cp_seqno"] : 쿠폰 일련번호
     * return : resultset 
     */ 
    function selectCpIssueList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    SELECT   A.office_nick";
        $query .= "\n            ,A.member_seqno";
        $query .= "\n            ,B.cp_num";
        $query .= "\n      FROM   member A";
        $query .= "\n            ,cp_issue B";
        $query .= "\n            ,cp C";
        $query .= "\n     WHERE   A.member_seqno = B.member_seqno";
        $query .= "\n       AND   B.cp_seqno = C.cp_seqno";

        //쿠폰 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"cp_seqno")) {

            $query .= "\n       AND   B.cp_seqno =" . $param["cp_seqno"];
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 회원 list select 
     * $conn : db connection
     * $param["cpn_seqno"] : 회사 관리 일련번호
     * return : resultset 
     */ 
    function selectMemberNickList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    SELECT   office_nick";
        $query .= "\n            ,member_seqno";
        $query .= "\n      FROM   member A";
        $query .= "\n     WHERE   cpn_admin_seqno=" . $param["cpn_seqno"];
        //그룹 아이디가 없을때
        $query .= "\n       AND   (group_id = ''";
        $query .= "\n        OR    group_id IS NULL)";
        //정상회원만
        $query .= "\n       AND   withdraw_dvs = '1'";
        

        //회원명 검색 있을때
        if ($this->blankParameterCheck($param ,"search")) {

            $search_str = substr($param["search"], 1, -1); 

            $query .= "\n       AND  office_nick like '%" . $search_str . "%' ";
        }

        $query .= "\n  ORDER BY member_seqno";

        $result = $conn->Execute($query);

        return $result;
    }


    /**
     * @brief 쿠폰 통계 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCpStatsList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  coupon_seqno";
        $query .= "\n           ,release_date ";
        $query .= "\n           ,expired_date ";
        $query .= "\n           ,detail ";
        $query .= "\n           ,discount_rate ";
        $query .= "\n           ,sell_site ";
        $query .= "\n      FROM  coupon";
        $query .= "\n     WHERE  1=1 ";

        return $conn->Execute($query);
    }
    
    /**
     * @brief 쿠폰 통계 카테고리 조회
     * 
     * @detail 
     *
     * @param $conn  = 디비 커넥션
     * @param $cp_seqno = 쿠폰번호
     *
     * @return
     */
    function selectCpStatsCate($conn, $cp_seqno) {

        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }
        $query  = "\n SELECT cate_name";
        $query .= "\n   FROM cp_cate cc, cate c";
        $query .= "\n  WHERE cc.cp_cate_sortcode = c.sortcode";
        $query .= "\n    AND cc.cp_seqno = ". $cp_seqno;

        return $conn->Execute($query);
    }

    /**
     * @brief 쿠폰 카테고리 조회
     * 
     * @param $conn     = 디비 커넥션
     * @param $cp_seqno = 쿠폰 일련번호
     *
     * @return 검색결과
     */
    function selectCpCate($conn, $cp_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $cp_seqno = $this->parameterEscape($conn, $cp_seqno);

        $query  = "\n SELECT cp_cate_sortcode";
        $query .= "\n   FROM cp_cate AS A";
        $query .= "\n  WHERE A.cp_seqno = ". $cp_seqno;

        return $conn->Execute($query);
    }
}
?>
