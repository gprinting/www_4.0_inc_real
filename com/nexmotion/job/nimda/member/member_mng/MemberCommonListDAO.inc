<?
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/MemberCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/nimda/member/member_mng/MemberCommonList.inc");
include_once(INC_PATH . "/com/nexmotion/doc/nimda/member/member_mng/MemberCommonInfo.inc");

/**
 * @file MemberCommonListDAO.inc
 *
 * @brief 회원 - 회원관리 - 회원통합리스트 DAO
 */
class MemberCommonListDAO extends MemberCommonDAO {

    function __construct() {
    }

    /**
     * @brief 회원통합리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberInfo($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        if($param["keyword"] == "") $search_dvs = "";

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  ="\nSELECT  A.member_seqno ";
            $query .="\n       ,A.member_name ";
            $query .="\n       ,A.sell_channel ";
            $query .="\n       ,A.dlvr_add_info ";
            $query .="\n       ,A.join_path ";
            $query .="\n       ,A.addr ";
            $query .="\n       ,A.addr_detail ";
            //$query .="\n       ,A.office_nick ";
            $query .="\n       ,A.tel_num ";
            $query .="\n       ,A.cell_num ";
            //$query .="\n       ,A.member_typ ";
            $query .="\n       ,A.grade ";
            //$query .="\n       ,A.office_eval ";
            $query .="\n       ,B.erp_grade_name ";
            $query .="\n       ,C.corp_name ";
            $query .="\n       ,C.repre_name ";
            $query .="\n       ,C.crn ";
        }

        $query .="\n  FROM  member A ";
        $query .="\n  INNER JOIN member_grade_policy B ON A.grade = B.grade ";
        $query .="\n  LEFT JOIN licensee_info C ON A.member_seqno = C.member_seqno";
        $query .="\n  WHERE 1 = 1 ";

        if($search_dvs == "member_name") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND A.member_name LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if($search_dvs == "id") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND A.id LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if($search_dvs == "business_name") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND C.corp_name LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if($search_dvs == "business_master") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND C.repre_name LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if($search_dvs == "business_license") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND C.crn LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if($search_dvs == "tel_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   AND A.tel_num LIKE '%";
                $query .= substr($param["keyword"], 1, -1) . "%'";
            }
        }

        if ($this->blankParameterCheck($param ,"version")) {
            $query .= "\n    AND  A.join_path = " . $param["version"];
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);
    }

    /**
     * @brief 회원정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberCommonInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n   SELECT  T1.* ";
        $query .= "\n          ,T2.fix_oa ";
        $query .= "\n          ,T2.bad_oa ";
        $query .= "\n          ,T2.loan_limit_price ";
        $query .= "\n     FROM ( ";
        $query .= "\n               SELECT  A.member_seqno ";
        $query .= "\n                      ,A.member_name ";
        $query .= "\n                      ,A.member_id ";
        $query .= "\n                      ,A.member_dvs ";
        $query .= "\n                      ,B.sell_site ";
        $query .= "\n                      ,A.cell_num ";
        $query .= "\n                      ,A.tel_num ";
        $query .= "\n                      ,A.mail ";
        $query .= "\n                      ,A.birth ";
        $query .= "\n                      ,A.new_yn ";
        $query .= "\n                      ,A.member_typ ";
        $query .= "\n                      ,A.onefile_etprs_yn ";
        $query .= "\n                      ,A.card_pay_yn ";
        $query .= "\n                 FROM  member AS A ";
        $query .= "\n                      ,cpn_admin AS B ";
        $query .= "\n                WHERE  A.cpn_admin_seqno = B.cpn_admin_seqno) AS T1 ";
        $query .= "\nLEFT JOIN  excpt_member AS T2 ";
        $query .= "\n       ON  T1.member_seqno = T2.member_seqno ";

        //회원일련번호
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n    WHERE  T1.member_seqno = $param[member_seqno] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 배송정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberDlvrInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  dlvr_friend_yn ";
        $query .= "\n       ,dlvr_friend_main ";
        $query .= "\n       ,dlvr_dvs ";
        $query .= "\n       ,order_way ";
        $query .= "\n       ,dlvr_code ";
        $query .= "\n       ,direct_dlvr_yn ";
        $query .= "\n  FROM  member ";
        $query .= "\n WHERE  1 = 1 ";

        //회원일련번호
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  member_seqno = $param[member_seqno] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 배송관리 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberDlvrWay($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  order_way = %s ";
        $query .= "\n           ,dlvr_dvs = %s ";
        $query .= "\n           ,dlvr_code = %s ";
        $query .= "\n           ,direct_dlvr_yn = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["order_way"], $param["dlvr_dvs"],
                         $param["dlvr_code"], $param["is_use_direct"], $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }
 

    /**
     * @brief 회원 기본정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberBasicInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");;

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  member_dvs = %s ";
        $query .= "\n           ,tel_num = %s ";
        $query .= "\n           ,cell_num = %s ";
        $query .= "\n           ,mail = %s ";
        $query .= "\n           ,birth = %s ";
        $query .= "\n           ,member_typ = %s ";
        $query .= "\n           ,onefile_etprs_yn = %s ";
        $query .= "\n           ,card_pay_yn = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["member_dvs"], $param["tel_num"], $param["cell_num"],
                         $param["mail"], $param["birth"], 
                         $param["member_typ"], $param["onefile_etprs_yn"], 
                         $param["card_pay_yn"], $param["final_modi_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }
 
    /**
     * @brief 회원요약정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberSummaryInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  A.member_id ";
        $query .= "\n       ,A.office_nick ";
        $query .= "\n       ,A.mailing_yn ";
        $query .= "\n       ,A.sms_yn ";
        $query .= "\n       ,B.grade_name ";
        $query .= "\n       ,A.member_typ ";
        $query .= "\n       ,A.own_point ";
        $query .= "\n       ,A.office_eval ";
        $query .= "\n       ,A.first_join_date ";
        $query .= "\n       ,A.first_order_date ";
        $query .= "\n       ,A.final_order_date ";
        $query .= "\n       ,A.nc_release_resp ";
        $query .= "\n       ,A.bl_release_resp ";
        $query .= "\n       ,A.cpn_admin_seqno ";
        $query .= "\n  FROM  member A ";
        $query .= "\n       ,member_grade_policy B ";
        $query .= "\n WHERE  A.grade = B.grade ";

        //회원일련번호
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = $param[member_seqno] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원정보 기본정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberDetailBasicInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");;

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  office_nick = %s ";
        $query .= "\n           ,mailing_yn = %s ";
        $query .= "\n           ,sms_yn = %s ";
        $query .= "\n           ,state = %s ";
        $query .= "\n           ,office_eval = %s ";
        $query .= "\n           ,nc_release_resp = %s ";
        $query .= "\n           ,bl_release_resp = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["office_nick"], $param["mailing_yn"],
                         $param["sms_yn"], $param["member_state"], $param["office_eval"], 
                         $param["nc_release_resp"], $param["bl_release_resp"], 
                         $param["final_modi_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원정보 현금영수증정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberDetailCashInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  cashreceipt_card_num = %s ";
        $query .= "\n           ,cashreceipt_cell_num = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["cashreceipt_card_num"],
                         $param["cashreceipt_cell_num"],
                         $param["final_modi_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원정보탭 - 정산정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberCalculInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT  A.prepay_price ";
        $query .= "\n          ,A.order_lack_price ";
        $query .= "\n          ,A.member_name ";
        $query .= "\n          ,B.bank_name ";
        $query .= "\n          ,B.ba_num ";
        $query .= "\n     FROM  member  AS A ";
        $query .= "\nLEFT JOIN  virt_ba_admin AS B ";
        $query .= "\n       ON  A.member_seqno = B.member_seqno ";
        $query .= "\n    WHERE  1 = 1 ";

        //회원일련번호
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n      AND  A.member_seqno = $param[member_seqno] ";
        }
 
        return $conn->Execute($query);
    }

    /**
     * @brief 회원정보 회원탈퇴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberWithdraw($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  withdraw_dvs = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n           ,own_point = NULL ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, 3,
                         $param["final_modi_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원매출리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberSalesInfo($conn, $dvs, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "TOTAL") {
            $query  ="\nSELECT  SUM(pay_price) AS sum ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.order_num ";
            $query .= "\n       ,A.title";
            $query .= "\n       ,A.order_detail ";
            $query .= "\n       ,A.amt ";
            $query .= "\n       ,A.count ";
            $query .= "\n       ,A.pay_price ";
            $query .= "\n       ,A.order_regi_date ";
            $query .= "\n       ,A.order_state ";
            $query .= "\n       ,A.order_common_seqno ";
        }

        $query .= "\n  FROM order_common AS A ";
        $query .= "\n WHERE 1 = 1 ";
        $query .= "\n   AND order_state > 1300 ";
 
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  A.member_seqno = $param[member_seqno] ";
        }

        //최근로인일자, 최초가입일, 최근주문일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val > $param[from] ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val <= $param[to] ";
        }

        if ($this->blankParameterCheck($param ,"search_txt")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.title LIKE '%" . $val . "%'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
 
        if ($this->blankParameterCheck($param ,"sorting")) {
            $sorting = substr($param["sorting"], 1, -1);
            $query .= "\n ORDER BY " . $sorting;

            if ($this->blankParameterCheck($param ,"sorting_type")) {
                $sorting_type = substr($param["sorting_type"], 1, -1);
                $query .= " " . $sorting_type . ", A.order_common_seqno DESC";
            }
        } else {
            $query .= "\n ORDER BY A.order_common_seqno DESC";
        }

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원포인트 지급내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPointReqInfo($conn, $dvs, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.point_name ";
            $query .= "\n       ,A.point ";
            $query .= "\n       ,A.req_empl_name ";
            $query .= "\n       ,A.aprvl_empl_name ";
            $query .= "\n       ,A.reason ";
            $query .= "\n       ,A.state ";
        }

        $query .= "\n  FROM  member_point_req AS A ";
        $query .= "\n WHERE 1 = 1 ";
 
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  A.member_seqno = $param[member_seqno] ";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
 
        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원포인트 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPointInfo($conn, $dvs, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query .= "\nSELECT  A.regi_date ";
            $query .= "\n       ,A.point_name ";
            $query .= "\n       ,A.point ";
            $query .= "\n       ,A.rest_point ";
            $query .= "\n       ,A.order_price ";
            $query .= "\n       ,A.give_reason ";
            $query .= "\n       ,A.dvs ";
            $query .= "\n       ,A.note ";
        }

        $query .= "\n  FROM  member_point_history AS A ";
        $query .= "\n WHERE 1 = 1 ";
 
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  A.member_seqno = $param[member_seqno] ";
        }

        //거래일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val > $param[from] ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val <= $param[to] ";
        }

        if ($this->blankParameterCheck($param ,"search_txt")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.point_name LIKE '%" . $val . "%'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
 
        if ($this->blankParameterCheck($param ,"sorting")) {
            $sorting = substr($param["sorting"], 1, -1);
            $query .= "\n ORDER BY " . $sorting;

            if ($this->blankParameterCheck($param ,"sorting_type")) {
                $sorting_type = substr($param["sorting_type"], 1, -1);
                $query .= " " . $sorting_type . " ,A.member_point_history_seqno DESC";
            }
        } else {
            $query .= "\n ORDER BY A.member_point_history_seqno DESC";
        }

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원쿠폰 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberCouponInfo($conn, $dvs, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.cp_name";
            $query .= "\n       ,A.val";
            $query .= "\n       ,A.unit";
            $query .= "\n       ,A.max_sale_price";
            $query .= "\n       ,A.min_order_price";
            $query .= "\n       ,B.use_deadline";
            $query .= "\n       ,B.issue_date";
            $query .= "\n       ,B.use_yn";
        }
        $query .= "\n  FROM  cp AS A";
        $query .= "\n       ,cp_issue AS B";
        $query .= "\n WHERE  A.cp_seqno = B.cp_seqno";   


        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  B.member_seqno = $param[member_seqno] ";
        }

        //발급일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val > $param[from] ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val <= $param[to] ";
        }

        if ($this->blankParameterCheck($param ,"search_txt")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.cp_name LIKE '%" . $val . "%'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
 
        if ($this->blankParameterCheck($param ,"sorting")) {
            $sorting = substr($param["sorting"], 1, -1);
            $query .= "\n ORDER BY " . $sorting;

            if ($this->blankParameterCheck($param ,"sorting_type")) {
                $sorting_type = substr($param["sorting_type"], 1, -1);
                $query .= " " . $sorting_type . ", cp_issue_seqno DESC";
            }
        } else {
            $query .= "\n ORDER BY cp_issue_seqno DESC";
        }

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원이벤트 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberEventInfo($conn, $dvs, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.event_typ ";
            $query .= "\n       ,A.prdt_name ";
            $query .= "\n       ,A.bnf ";
            $query .= "\n       ,A.regi_date ";
        }
        
        $query .= "\n  FROM  member_event AS A ";
        $query .= "\n WHERE  1 = 1 ";
 
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n   AND  A.member_seqno = $param[member_seqno] ";
        }

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val > $param[from] ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  A.$val <= $param[to] ";
        }

        if ($this->blankParameterCheck($param ,"search_txt")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.event_typ LIKE '%" . $val . "%'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
 
        if ($this->blankParameterCheck($param ,"sorting")) {
            $sorting = substr($param["sorting"], 1, -1);
            $query .= "\n ORDER BY " . $sorting;

            if ($this->blankParameterCheck($param ,"sorting_type")) {
                $sorting_type = substr($param["sorting_type"], 1, -1);
                $query .= " " . $sorting_type . " ,member_event_seqno DESC";
            }
        } else {
            $query .= "\n ORDER BY member_event_seqno DESC";
        }

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원에게 부여 된 가상계좌 반환
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateVirtBaAdmin($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  virt_ba_admin ";
        $query .= "\n       SET  cpn_admin_seqno = %s ";
        $query .= "\n           ,member_seqno = %s ";
        $query .= "\n           ,state = 'N' ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, 'NULL',
                         'NULL',
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원 등급 수동여부
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateMemberAutoGrade($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  auto_grade_yn = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["auto_grade_yn"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 일반 접수팀
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectGeneInfo($conn, $param, $is_all = false) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $val = substr($param["depar_code"], 1, -1);

        $query  = "\nSELECT  A.depar_name ";
        $query .= "\n       ,A.depar_code ";
        $query .= "\n       ,A.high_depar_code ";
        $query .= "\n  FROM  depar_admin AS A ";
        $query .= "\n WHERE  1 = 1";
        if (!$is_all) {
            $query .= "\n   AND  high_depar_code = '003'";
        }
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = $param[sell_site] ";
        }
        $query .= "\n ORDER BY depar_level";

        return $conn->Execute($query);
    }

    /**
     * @brief 회원에 지정되어있는 담당자 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberMngInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 사내담당자 정보 */";
        $query .= "\n         A.member_mng_seqno";
        $query .= "\n        ,A.ca";
        $query .= "\n        ,A.crm";
        $query .= "\n        ,A.cs";
        $query .= "\n        ,A.pro";
        $query .= "\n   FROM  member_mng AS A ";
        $query .= "\n  WHERE  A.member_seqno =  %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    ////////////////////////////////////////////////////////////////
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ////////////////////////////////////////////////////////////////
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    ////////////////////////////////////////////////////////////////

    /**
     * @brief 회원 상세정보 팝업 - 회원 기본정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberDefaultInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 회원기본정보 */";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.member_dvs";
        $query .= "\n        ,A.member_typ";
        $query .= "\n        ,A.grade";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,A.fax_num";
        $query .= "\n        ,A.mail";
        $query .= "\n        ,A.mailing_yn";
        $query .= "\n        ,A.sms_yn";
        $query .= "\n        ,A.zipcode";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";
        $query .= "\n        ,A.first_join_date";
        $query .= "\n        ,A.final_modi_date";
        $query .= "\n        ,A.card_pay_yn";
        $query .= "\n        ,A.prepay_price_money";
        $query .= "\n        ,A.refund_ba_num";
        $query .= "\n        ,A.refund_bank_name";
        $query .= "\n        ,A.refund_name";
        $query .= "\n        ,A.direct_dlvr_yn";
        $query .= "\n        ,A.dlvr_add_info";
        $query .= "\n        ,A.default_release";
        $query .= "\n        ,A.is_except";
        $query .= "\n        ,B.erp_grade_name";

        $query .= "\n   FROM  member AS A";
        $query .= "\n   INNER JOIN  member_grade_policy AS B ON A.grade = B.grade";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);
        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 상세정보 팝업 - 기업회원 사업자등록정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberLicenseeInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 기업회원 사업자등록정보 */";
        $query .= "\n         A.licensee_info_seqno";
        $query .= "\n        ,A.corp_name";
        $query .= "\n        ,A.repre_name";
        $query .= "\n        ,A.crn";
        $query .= "\n        ,A.bc";
        $query .= "\n        ,A.tob";
        $query .= "\n        ,A.zipcode";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";

        $query .= "\n   FROM  licensee_info AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 상세정보 팝업 - 결제정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberVirtBaInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 결제정보 */";
        $query .= "\n         A.ba_num";
        $query .= "\n        ,A.bank_name";
        $query .= "\n        ,A.depo_name";

        $query .= "\n   FROM  virt_ba_admin AS A";
        //$query .= "\n   FROM  virt_ba_admin_test AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n  AND A.use_yn = 'Y' ";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 상세정보 팝업 - 사내 담당자 정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEmplInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $depar_code = $this->arr2paramStr($conn, $param["depar_code_arr"]);

        $query .= "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 사내 담당자 정보 */";
        $query .= "\n         A.empl_seqno";
        $query .= "\n        ,A.name";
        $query .= "\n        ,A.depar_code";
        $query .= "\n        ,A.posi_code";
        $query .= "\n        ,A.job_code";

        $query .= "\n   FROM  empl A";
        $query .= "\n  WHERE  A.depar_code IN (%s)";

        $query  = sprintf($query, $depar_code);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 기업회원 회계담당자 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAcctingMngInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 회원등록 회계 담당자 정보 */";
        $query .= "\n        A.name";
        $query .= "\n       ,A.posi";
        $query .= "\n       ,A.mail";
        $query .= "\n       ,A.tel_num";
        $query .= "\n       ,A.cell_num";
        $query .= "\n  FROM  accting_mng A";
        $query .= "\n WHERE  A.member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 기업회원 관리사업자 등록증 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAdminLicenseeregiInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 추가 사업자 등록 정보 */";
        $query .= "\n        A.admin_licenseeregi_seqno";
        $query .= "\n       ,A.crn";
        $query .= "\n       ,A.corp_name";
        $query .= "\n       ,A.repre_name";
        $query .= "\n       ,A.bc";
        $query .= "\n       ,A.tob";
        $query .= "\n       ,A.tel_num";
        $query .= "\n       ,A.fax_num";
        $query .= "\n       ,A.zipcode";
        $query .= "\n       ,A.addr";
        $query .= "\n       ,A.addr_detail";
        $query .= "\n  FROM  admin_licenseeregi A";
        $query .= "\n WHERE  A.member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 직원이 입력한 회원 담당자 정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEmplInputMemberMngInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 직원 입력 회원 담당자 정보 */";
        $query .= "\n        A.empl_input_member_mng_seqno";
        $query .= "\n       ,A.name";
        $query .= "\n       ,A.cpn_tel_num";
        $query .= "\n       ,A.cell_num";
        $query .= "\n       ,A.job";
        $query .= "\n       ,A.depar";
        $query .= "\n       ,A.exten";
        $query .= "\n       ,A.mail";
        $query .= "\n       ,A.pro";
        $query .= "\n       ,A.zipcode";
        $query .= "\n       ,A.addr";
        $query .= "\n       ,A.addr_detail";
        $query .= "\n  FROM  empl_input_member_mng A";
        $query .= "\n WHERE  A.member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 직원이 입력한 회원 담당자 정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberCertiInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 회원 인증 정보 */";
        $query .= "\n         A.member_certi_seqno";
        $query .= "\n        ,A.file_path";
        $query .= "\n        ,A.save_file_name";
        $query .= "\n        ,A.origin_file_name";
        $query .= "\n   FROM  member_certi AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param ,"member_seqno")) { 
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param ,"member_certi_seqno")) { 
            $query .= "\n   AND  A.member_certi_seqno = " . $param["member_certi_seqno"];
        }

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 회원 상세정보 팝업 - 회원정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 회원정보 수정 */";
        $query .= "\n         member";
        $query .= "\n    SET  office_nick          = %s";
        $query .= "\n        ,member_name           = %s";
        $query .= "\n        ,member_dvs           = %s";
        $query .= "\n        ,member_typ           = %s";
        $query .= "\n        ,grade                = %s";
        $query .= "\n        ,tel_num              = %s";
        $query .= "\n        ,cell_num             = %s";
        $query .= "\n        ,fax_num              = %s";
        $query .= "\n        ,mail                 = %s";
        $query .= "\n        ,mailing_yn           = %s";
        $query .= "\n        ,sms_yn               = %s";
        $query .= "\n        ,zipcode              = %s";
        $query .= "\n        ,addr                 = %s";
        $query .= "\n        ,addr_detail          = %s";
        $query .= "\n        ,direct_dlvr_yn             = %s";
        $query .= "\n        ,dlvr_add_info             = %s";
        $query .= "\n        ,card_pay_yn          = %s";
        $query .= "\n        ,refund_ba_num        = %s";
        $query .= "\n        ,refund_bank_name     = %s";
        $query .= "\n        ,refund_name          = %s";
        $query .= "\n        ,default_release          = %s";
        $query .= "\n        ,is_except          = %s";
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["office_nick"]
                                ,$param["office_nick"]
                                , $param["member_dvs"]
                                , $param["member_typ"]
                                , $param["grade"]
                                , $param["tel_num"]
                                , $param["cell_num"]
                                , $param["fax_num"]
                                , $param["mail"]
                                , $param["mailing_yn"]
                                , $param["sms_yn"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]
                                , $param["direct_dlvr_yn"]
                                , $param["dlvr_add_info"]
                                , $param["card_pay_yn"]
                                , $param["refund_ba_num"]
                                , $param["refund_bank_name"]
                                , $param["refund_name"]
                                , $param["default_release"]
                                , $param["is_except"]
                                , $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 사업자 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateLicenseeInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 사업자정보 수정 */";
        $query .= "\n        licensee_info (";
        $query .= "\n      corp_name";
        $query .= "\n     ,repre_name";
        $query .= "\n     ,crn";
        $query .= "\n     ,bc";
        $query .= "\n     ,tob";
        $query .= "\n     ,zipcode";
        $query .= "\n     ,addr";
        $query .= "\n     ,addr_detail";
        $query .= "\n     ,member_seqno";
        if ($this->blankParameterCheck($param ,"licensee_info_seqno")) {
            $query .= "\n     ,licensee_info_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"licensee_info_seqno")) {
            $query .= "\n     ," . $param["licensee_info_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      corp_name   = %s";
        $query .= "\n     ,repre_name  = %s";
        $query .= "\n     ,crn         = %s";
        $query .= "\n     ,bc          = %s";
        $query .= "\n     ,tob         = %s";
        $query .= "\n     ,zipcode     = %s";
        $query .= "\n     ,addr        = %s";
        $query .= "\n     ,addr_detail = %s";

        $query  = sprintf($query, $param["corp_name"]
                                , $param["repre_name"]
                                , $param["crn"]
                                , $param["bc"]
                                , $param["tob"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]
                                , $param["member_seqno"]
                                , $param["corp_name"]
                                , $param["repre_name"]
                                , $param["crn"]
                                , $param["bc"]
                                , $param["tob"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 관리 사업자 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateAdminLicenseeregiInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 관리사업자정보 수정 */";
        $query .= "\n        admin_licenseeregi (";
        $query .= "\n      corp_name";
        $query .= "\n     ,repre_name";
        $query .= "\n     ,crn";
        $query .= "\n     ,bc";
        $query .= "\n     ,tob";
        $query .= "\n     ,tel_num";
        $query .= "\n     ,fax_num";
        $query .= "\n     ,zipcode";
        $query .= "\n     ,addr";
        $query .= "\n     ,addr_detail";
        $query .= "\n     ,member_seqno";
        if ($this->blankParameterCheck($param ,"admin_licenseeregi_seqno")) {
            $query .= "\n     ,admin_licenseeregi_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"admin_licenseeregi_seqno")) {
            $query .= "\n     ," . $param["admin_licenseeregi_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      corp_name   = %s";
        $query .= "\n     ,repre_name  = %s";
        $query .= "\n     ,crn         = %s";
        $query .= "\n     ,bc          = %s";
        $query .= "\n     ,tob         = %s";
        $query .= "\n     ,tel_num     = %s";
        $query .= "\n     ,fax_num     = %s";
        $query .= "\n     ,zipcode     = %s";
        $query .= "\n     ,addr        = %s";
        $query .= "\n     ,addr_detail = %s";

        $query  = sprintf($query, $param["corp_name"]
                                , $param["repre_name"]
                                , $param["crn"]
                                , $param["bc"]
                                , $param["tob"]
                                , $param["tel_num"]
                                , $param["fax_num"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]
                                , $param["member_seqno"]
                                , $param["corp_name"]
                                , $param["repre_name"]
                                , $param["crn"]
                                , $param["bc"]
                                , $param["tob"]
                                , $param["tel_num"]
                                , $param["fax_num"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 직원 입력 담당자 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateEmplInputMemberMngInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 관리사업자정보 수정 */";
        $query .= "\n        empl_input_member_mng (";
        $query .= "\n      name";
        $query .= "\n     ,cpn_tel_num";
        $query .= "\n     ,exten";
        $query .= "\n     ,cell_num";
        $query .= "\n     ,job";
        $query .= "\n     ,depar";
        $query .= "\n     ,mail";
        $query .= "\n     ,pro";
        $query .= "\n     ,member_seqno";
        if ($this->blankParameterCheck($param ,"empl_input_member_mng_seqno")) {
            $query .= "\n     ,empl_input_member_mng_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"empl_input_member_mng_seqno")) {
            $query .= "\n     ," . $param["empl_input_member_mng_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      name     = %s";
        $query .= "\n     ,cpn_tel_num  = %s";
        $query .= "\n     ,exten    = %s";
        $query .= "\n     ,cell_num = %s";
        $query .= "\n     ,job      = %s";
        $query .= "\n     ,depar    = %s";
        $query .= "\n     ,mail     = %s";
        $query .= "\n     ,pro      = %s";

        $query  = sprintf($query, $param["name"]
                                , $param["cpn_tel_num"]
                                , $param["exten"]
                                , $param["cell_num"]
                                , $param["job"]
                                , $param["depar"]
                                , $param["mail"]
                                , $param["pro"]
                                , $param["member_seqno"]
                                , $param["name"]
                                , $param["cpn_tel_num"]
                                , $param["exten"]
                                , $param["cell_num"]
                                , $param["job"]
                                , $param["depar"]
                                , $param["mail"]
                                , $param["pro"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 예외회원 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateExcptMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 관리사업자정보 수정 */";
        $query .= "\n        excpt_member (";
        $query .= "\n      action_result";
        $query .= "\n     ,loan_pay_promi_date";
        $query .= "\n     ,loan_limit_price";
        $query .= "\n     ,loan_pay_promi_dvs";
        $query .= "\n     ,loan_pay_promi_dvs_day";
        $query .= "\n     ,member_seqno";
        if ($this->blankParameterCheck($param ,"excpt_member_seqno")) {
            $query .= "\n     ,excpt_member_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"excpt_member_seqno")) {
            $query .= "\n     ," . $param["excpt_member_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      action_result          = %s";
        $query .= "\n     ,loan_pay_promi_date    = %s";
        $query .= "\n     ,loan_limit_price       = %s";
        $query .= "\n     ,loan_pay_promi_dvs     = %s";
        $query .= "\n     ,loan_pay_promi_dvs_day = %s";

        $query  = sprintf($query, $param["action_result"]
                                , $param["loan_pay_promi_date"]
                                , $param["loan_limit_price"]
                                , $param["loan_pay_promi_dvs"]
                                , $param["loan_pay_promi_dvs_day"]
                                , $param["member_seqno"]
                                , $param["action_result"]
                                , $param["loan_pay_promi_date"]
                                , $param["loan_limit_price"]
                                , $param["loan_pay_promi_dvs"]
                                , $param["loan_pay_promi_dvs_day"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 사내담당자 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMemberMngInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 관리사업자정보 수정 */";
        $query .= "\n        member_mng (";
        $query .= "\n      resp_deparcode";
        //$query .= "\n     ,cm";
        $query .= "\n     ,ca";
        $query .= "\n     ,crm";
        $query .= "\n     ,cs";
        $query .= "\n     ,pro";
        $query .= "\n     ,member_seqno";
        if ($this->blankParameterCheck($param ,"member_mng_seqno")) {
            $query .= "\n     ,member_mng_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        //$query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"member_mng_seqno")) {
            $query .= "\n     ," . $param["member_mng_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      resp_deparcode = %s";
        //$query .= "\n     ,cm  = %s";
        $query .= "\n     ,ca  = %s";
        $query .= "\n     ,crm = %s";
        $query .= "\n     ,cs  = %s";
        $query .= "\n     ,pro = %s";

        $query  = sprintf($query, $param["resp_deparcode"]
                                //, $param["cm"]
                                , $param["ca"]
                                , $param["crm"]
                                , $param["cs"]
                                , $param["pro"]
                                , $param["member_seqno"]
                                , $param["resp_deparcode"]
                                //, $param["cm"]
                                , $param["ca"]
                                , $param["crm"]
                                , $param["cs"]
                                , $param["pro"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 상세정보 팝업 - 등록증 이미지 추가
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMemberCertiInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberCommonListDAO.inc 회원 - 회원상세팝업 - 사업자 등록증 이미지 추가 */";
        $query .= "\n        member_certi (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,file_path";
        $query .= "\n     ,save_file_name";
        $query .= "\n     ,origin_file_name";
        if ($this->blankParameterCheck($param ,"member_certi_seqno")) {
            $query .= "\n     ,member_certi_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        if ($this->blankParameterCheck($param ,"member_certi_seqno")) {
            $query .= "\n     ," . $param["member_certi_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      file_path        = %s";
        $query .= "\n     ,save_file_name   = %s";
        $query .= "\n     ,origin_file_name = %s";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["file_path"]
                                , $param["save_file_name"]
                                , $param["origin_file_name"]
                                , $param["file_path"]
                                , $param["save_file_name"]
                                , $param["origin_file_name"]);

        return $conn->Execute($query);
    }
}
?>
