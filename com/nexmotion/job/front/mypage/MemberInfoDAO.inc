<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/MypageCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/mypage/MemberInfoHTML.inc');

class MemberInfoDAO extends MypageCommonDAO {
 
    function __construct() {
    }

    /**
     * @brief 회원 가입 정보
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberJoinInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT mail ";
        $query .= "\n      ,tel_num ";
        $query .= "\n      ,cell_num ";
        $query .= "\n      ,zipcode ";
        $query .= "\n      ,addr ";
        $query .= "\n      ,addr_detail ";
        $query .= "\n      ,sms_yn ";
        $query .= "\n      ,mailing_yn ";
        $query .= "\n      ,join_path ";
        $query .= "\n  FROM member ";
        $query .= "\n WHERE member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 비밀번호 체크
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPw($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT passwd ";
        $query .= "\n  FROM member ";
        $query .= "\n WHERE member_seqno = " . $param["member_seqno"];
        $query .= "\n AND passwd = PASSWORD(" . $param["old_password"] . ")";

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

        //171213 이청산 : withdraw_dvs는 Y/N으로 변경
        /*
        $withdraw_dvs = "";
        if ($param["withdraw_dvs"]) {
            $withdraw_dvs = $param["withdraw_dvs"];
        } else {
            $withdraw_dvs = 3;
        }
        */
        $withdraw_dvs = '"Y"';

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  withdraw_yn = %s ";
        $query .= "\n           ,final_modi_date = %s ";
     // 171213 이청산 : 포인트 관련해선 추후 추가
     //   $query .= "\n           ,own_point = NULL ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $withdraw_dvs,
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
     * @brief 회원정보 회계담당자 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function deleteMemberAccounting($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    DELETE  ";
        $query .= "\n      FROM  accting_mng ";
        $query .= "\n     WHERE  accting_mng_seqno = %s ";

        $query = sprintf($query, $param["accting_mng_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원정보 회원탈퇴 연동계정정보삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function deleteMemberSubId($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    DELETE  ";
        $query .= "\n      FROM  member_sub_id ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 기업에 속한 회원 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCoPerMember($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  member_name ";
        $query .= "\n       ,id ";
        $query .= "\n       ,tel_num ";
        $query .= "\n       ,cell_num ";
        $query .= "\n       ,mail ";
        $query .= "\n       ,member_seqno ";
        $query .= "\n  FROM  member";
        $query .= "\n WHERE  group_id = " . $param["member_seqno"];
        $query .= "\n   AND  withdraw_yn = 'N'";

        return $conn->Execute($query);
    }

    /**
     * @brief 가입정보 변경
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateMemberJoinInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  mail = %s ";
        //$query .= "\n           ,birth = %s ";
        $query .= "\n           ,tel_num = %s ";
        $query .= "\n           ,cell_num = %s ";
        $query .= "\n           ,zipcode = %s ";
        $query .= "\n           ,addr = %s ";
        $query .= "\n           ,addr_detail = %s ";
        $query .= "\n           ,mailing_yn = %s ";
        $query .= "\n           ,sms_yn = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["mail"],
                         //$param["birth"],
                         $param["tel_num"],
                         $param["cell_num"],
                         $param["zipcode"],
                         $param["addr"],
                         $param["addr_detail"],
                         $param["mailing_yn"],
                         $param["sms_yn"],
                         $param["final_modi_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief 기업 주문담당자 회원정보 등록
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertCoOrderMng($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["first_join_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = " \n INSERT INTO member(";
        $query .= " \n      group_id";
        $query .= " \n     ,group_name";
        $query .= " \n     ,id";
        $query .= " \n     ,member_name";
        $query .= " \n     ,member_dvs";
        $query .= " \n     ,passwd";
        $query .= " \n     ,tel_num";
        $query .= " \n     ,cell_num";
        $query .= " \n     ,first_join_date";
        $query .= " \n     ,mail";
        $query .= " \n     ,withdraw_yn";
        $query .= " \n ) VALUES (";
        $query .= " \n      %s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,'N'";
        $query .= " \n )";

        $query  = sprintf($query, $param["group_id"]
                                , $param["group_name"]
                                , $param["member_id"]
                                , $param["member_name"]
                                , $param["member_dvs"]
                                , $param["passwd"]
                                , $param["tel_num"]
                                , $param["cell_num"]
                                , $param["first_join_date"]
                                , $param["mail"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }
 
    /**
     * @brief 기업에 속한 회원 상세정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCoPerMemberInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  member_name ";
        $query .= "\n       ,posi ";
        $query .= "\n       ,member_id ";
        $query .= "\n       ,tel_num ";
        $query .= "\n       ,cell_num ";
        $query .= "\n       ,mail ";
        $query .= "\n  FROM  member";
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 가상계좌정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberVirtBa($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* 회원 가상계좌정보 검색 query */ ";
        $query .= "\n        virt_ba_admin_seqno ";
        $query .= "\n       ,bank_name ";
        $query .= "\n       ,depo_name ";
        $query .= "\n  FROM  virt_ba_admin"; // TODO 추후 이부분 수정필요
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];
        $query .= "\n   AND  use_yn = " . $param["use_yn"];

        return $conn->Execute($query);
    }

    /**
     * @brief 빈 가상계좌정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectVacantVirtBa($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* 회원 가상계좌정보 query */ ";
        $query .= "\n        virt_ba_admin_seqno ";
        $query .= "\n       ,ba_num ";
        $query .= "\n       ,bank_name ";
        $query .= "\n  FROM  virt_ba_admin"; // TODO 추후 이부분 수정필요
        $query .= "\n WHERE  bank_name = " . $param["bank_name"];
        $query .= "\n   AND  use_yn = " . $param["use_yn"];
        $query .= "\n LIMIT  1 ";

        return $conn->Execute($query);
    }

    /**
     * @brief 가상계좌 변경정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrevChangeList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* 회원 가상계좌 변경정보 검색 query */ ";
        $query .= "\n        virt_ba_change_history_seqno ";
        $query .= "\n       ,change_date ";
        $query .= "\n       ,depo_name ";
        $query .= "\n       ,bank_before ";
        $query .= "\n       ,bank_aft ";
        $query .= "\n       ,prog_state ";
        $query .= "\n       ,progday ";
        $query .= "\n       ,cancel_yn ";
        $query .= "\n  FROM  virt_ba_change_history";
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];
        //진행_상태
        if ($this->blankParameterCheck($param ,"prog_state")) {
            $query .= "\n   AND  prog_state = " . $param["prog_state"];
        }
        //취소_상태 확인을 위한 seqno
        if ($this->blankParameterCheck($param ,"seqno")) {
            $query .= "\n   AND  virt_ba_change_history_seqno = " . $param["seqno"];
        }
        // 내역 MAX 5건 불러오기용 param
        if ($this->blankParameterCheck($param ,"maxfive")) {
            $query .= "\n   ORDER BY virt_ba_change_history_seqno DESC ";
            $query .= "\n   LIMIT 5 ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 가상계좌 변경 신청정보 등록
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertVirtBaChangeHistory($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = " \n INSERT INTO virt_ba_change_history(";
        $query .= " \n      member_seqno";
        $query .= " \n     ,change_date";
        $query .= " \n     ,depo_name";
        $query .= " \n     ,bank_before";
        $query .= " \n     ,bank_aft";
        $query .= " \n     ,prog_state";
        $query .= " \n     ,cancel_yn";
        $query .= " \n ) VALUES (";
        $query .= " \n      %s";
        $query .= " \n     ,now()";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n     ,%s";
        $query .= " \n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["depo_name"]
                                , $param["bank_before"]
                                , $param["bank_aft"]
                                , $param["prog_state"]
                                , $param["cancel_yn"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 가상계좌 업데이트
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateMemberVirtBa($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  virt_ba_admin "; // TODO 수정필요
        $query .= "\n       SET  member_seqno = %s ";
        $query .= "\n           ,depo_name    = %s ";
        $query .= "\n           ,use_yn       = %s ";
        $query .= "\n     WHERE  virt_ba_admin_seqno = %s ";

        $query = sprintf($query, $param["member_seqno"]
                               , $param["depo_name"]
                               , $param["use_yn"]
                               , $param["vbas"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief 가상계좌 변경취소 업데이트
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateVirtBaChangeHistory($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  virt_ba_change_history ";
        $query .= "\n       SET  cancel_yn  = %s ";
        $query .= "\n           ,prog_state = %s ";
        $query .= "\n     WHERE  virt_ba_change_history_seqno = %s ";

        $query = sprintf($query, $param["cancel_yn"]
                               , $param["prog_state"]
                               , $param["seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief 가상계좌 사용정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectVirtBaMember($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* 가상계좌 사용정보 query */ ";
        $query .= "\n        member_seqno ";
        $query .= "\n  FROM  virt_ba_admin"; // TODO 추후 이부분 수정필요
        $query .= "\n WHERE  bank_name = " . $param["bank_name"];
        $query .= "\n   AND  ba_num = " . $param["bank_num"];

        return $conn->Execute($query);
    }

    /**
     * @brief 월배송 조건 확인
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberMonthlySales($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberInfoDAO.inc - 월배송 query */ ";
        $query .= "\n         SUM(A.net_sales_price) AS net";
        $query .= "\n        ,SUM(A.card_net_sales_price) AS card_net";
        $query .= "\n   FROM  day_sales_stats AS A";
        $query .= "\n  WHERE  A.member_seqno = ";
        $query .= $param["member_seqno"];
        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .="\n      AND  A.input_date >= '" . $from . "'";
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .="\n      AND  A.input_date <= '" . $to . "'";
        }

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 기업에 속한 회원 가입정보 수정
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateCoOrderMng($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_modi_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  mail = %s ";
        $query .= "\n           ,member_name = %s ";
        $query .= "\n           ,passwd = %s ";
        $query .= "\n           ,tel_num = %s ";
        $query .= "\n           ,cell_num = %s ";
        $query .= "\n           ,final_modi_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["mail"],
                         $param["member_name"],
                         $param["passwd"],
                         $param["tel_num"],
                         $param["cell_num"],
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
     * @brief 쿠폰 내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCpList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $today = date("Y-m-d H:i:s", time());

        if ($type === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
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
        $query .= "\n   AND  B.member_seqno = " . $param["seqno"];
        $query .= "\n   AND  B.use_yn = 'N'";
        $query .= "\n   AND  B.use_able_start_date <= '" . $today . "'";

        //상태
        if ($this->blankParameterCheck($param ,"state")) {
            
            $state = substr($param["state"], 1, -1);

            //미사용 사용가능한 쿠폰
            if ($state == 1) {
                $query .= "\n       AND B.use_able_start_date <= '";
                $query .= $today . "'";
                $query .= "\n       AND (B.use_deadline >= '" . $today . "'";
                $query .= "\n        OR B.use_deadline is NULL)";

            //기한 만료된 쿠폰
            } else  {
                $query .= "\n       AND (B.use_able_start_date > '";
                $query .= $today . "'";
                $query .= "\n        OR B.use_deadline < '" . $today . "')";
            }
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($type == "SEQ") {

            $query .= "\n ORDER BY B.cp_issue_seqno DESC ";
            $query .= "\n LIMIT ". $s_num . ", " . $list_num;

        }

        return $conn->Execute($query);
    }

    /**
     * @brief 포인트 내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPointList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);

        if ($type === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n    SELECT  regi_date";
            $query .= "\n           ,point";
            $query .= "\n           ,rest_point";
            $query .= "\n           ,order_price";
            $query .= "\n           ,dvs";
            $query .= "\n           ,order_num";
        }
        $query .= "\n      FROM  member_point_history";
        $query .= "\n     WHERE  member_seqno = " . $param["seqno"];

        //구분
        if ($this->blankParameterCheck($param ,"dvs")) {

            $query .= "\n    AND  dvs = " . $param["dvs"];

        }

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {

            $from = substr($param["from"], 1, -1);

            $query .="\n     AND  regi_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {

            $to = substr($param["to"], 1, -1);

            $query .="\n     AND  regi_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($type == "SEQ") {

            $query .= "\n ORDER BY member_point_history_seqno DESC ";
            $query .= "\n LIMIT ". $s_num . ", " . $list_num;

        }

        return $conn->Execute($query);
    }

    /**
     * @brief 참여중인 이벤트 건수
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectJoinEventCount($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  COUNT(*) AS cnt ";
        $query .= "\n  FROM  member AS A ";
        $query .= "\n       ,order_common AS B";
        $query .= "\n WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n   AND  B.event_yn = 'Y'";
        $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 이벤트 내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEventList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $today = date("Y-m-d H:i:s", time());

        if ($type === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\nSELECT  member_event_seqno ";
            $query .= "\n       ,event_typ ";
            $query .= "\n       ,prdt_name ";
            $query .= "\n       ,bnf ";
            $query .= "\n       ,regi_date ";
        }
        $query .= "\n  FROM  member_event ";
        $query .= "\n WHERE  member_seqno = " . $param["seqno"];

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);

            $query .="\n     AND  regi_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);

            $query .="\n     AND  regi_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        //이벤트명
        if ($this->blankParameterCheck($param ,"event_typ")) {
            $val = substr($param["event_typ"], 1, -1);
            $query .="\n  AND  event_typ LIKE '%" . $val . "%'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
        $query .= "\n ORDER BY regi_date DESC ";
        $query .= "\n LIMIT ". $s_num . ", " . $list_num;

        return $conn->Execute($query);
    } 
 
    /**
     * @brief 카테고리 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateTable($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $sortcode = substr($param["cate_sortcode"], 1, -1);
        $type = substr($param["type"], 1, -1);
 
        if ($type === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\nSELECT  cate_name";
            $query .= "\n       ,sortcode";
        }
        $query .= "\n  FROM  cate";
        $query .= "\n WHERE  cate_level = " . $param["cate_level"];
        $query .= "\n   AND  sortcode LIKE '" . $sortcode . "%'";
        $query .= "\nORDER BY sortcode";

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 별 회원등급 할인정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateGradeInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션어택방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($type === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n   SELECT  B.rate";
        }
        $query .= "\n     FROM  member_grade_policy AS A";
        $query .= "\nLEFT JOIN  grade_sale_price AS B";
        $query .= "\n       ON  A.grade = B.grade";
        $query .= "\n    WHERE  1 = 1";

        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n      AND  B.cate_sortcode = ";
            $query .= $param["cate_sortcode"];
        }

        if ($this->blankParameterCheck($param, "grade")) {
            $query .= "\n      AND  A.grade = ";
            $query .= $param["grade"];
        }
        $query .= "\nORDER BY B.cate_sortcode";

        return $conn->Execute($query);
    }

    /**
     * @brief 선입금 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @modified at 2017/0728 by montvert : depo_way컬럼 삭제로 인한 쿼리수정
     *
     * @return 검색결과
     */
    function selectPrepaymentList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);

        if ($type === "COUNT") {
            $query  = "\n  SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n  SELECT  A.deal_date";
            $query .= "\n         ,A.card_depo_price";
            $query .= "\n         ,A.card_pay_price";
            $query .= "\n         ,A.state";
            $query .= "\n         ,A.deal_num";
            $query .= "\n         ,A.input_typ";
        }
        $query .= "\n    FROM  member_pay_history AS A";
        $query .= "\n   WHERE  A.member_seqno IN (" . $seqno . ")";
        //$query .= "\n     AND  (A.input_typ = '102' OR A.input_typ = '008')";
        $query .= "\n     AND  (A.input_typ = '102')";

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  A.deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  A.deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($type == "SEQ") {
            $query .= "\nORDER BY A.deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 소셜로그인 연동 확인
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectSocLinkInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  mail ";
        $query .= "\n       ,join_path ";
        $query .= "\n  FROM  member_sub_id";
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 기본배송지 일련번호 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectMemberBasicDlvrSeqno($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  member_dlvr_seqno";
        $query .= "\n   FROM  member_dlvr";
        $query .= "\n  WHERE  member_seqno = " . $member_seqno;
        $query .= "\n    AND  basic_yn = 'Y'";

        $rs = $conn->Execute($query)->fields;

        return $rs["member_dlvr_seqno"];
    }

    /**
     * @brief 회원 기본배송지 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 쿼리 실행결과
     */
    function updateMemberBasicDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* MemberInfoDAO.inc - 회원 기본배송지 정보 입력/수정 */";
        $query .= "\n             member_dlvr (";
        $query .= "\n      dlvr_name";
        $query .= "\n     ,recei";
        $query .= "\n     ,tel_num";
        $query .= "\n     ,cell_num";
        $query .= "\n     ,zipcode";
        $query .= "\n     ,addr";
        $query .= "\n     ,addr_detail";
        $query .= "\n     ,basic_yn";
        $query .= "\n     ,member_seqno";
        $query .= "\n     ,regi_date";
        if ($this->blankParameterCheck($param, "member_dlvr_seqno")) {
            $query .= "\n     ,member_dlvr_seqno";
        }
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,'Y'";
        $query .= "\n     ,%s";
        $query .= "\n     ,now()";
        if ($this->blankParameterCheck($param, "member_dlvr_seqno")) {
            $query .= "\n     ," . $param["member_dlvr_seqno"];
        }
        $query .= "\n ) ON DUPLICATE KEY UPDATE";
        $query .= "\n      dlvr_name = VALUES(dlvr_name)";
        $query .= "\n     ,recei = VALUES(recei)";
        $query .= "\n     ,tel_num = VALUES(tel_num)";
        $query .= "\n     ,cell_num = VALUES(cell_num)";
        $query .= "\n     ,zipcode = VALUES(zipcode)";
        $query .= "\n     ,addr = VALUES(addr)";
        $query .= "\n     ,addr_detail = VALUES(addr_detail)";
        $query .= "\n     ,basic_yn = VALUES(basic_yn)";
        $query .= "\n     ,member_seqno = VALUES(member_seqno)";
 
        $query = sprintf($query, $param["dlvr_name"]
                               , $param["recei"]
                               , $param["tel_num"]
                               , $param["cell_num"]
                               , $param["zipcode"]
                               , $param["addr"]
                               , $param["addr_detail"]
                               , $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 환불계좌정보 업데이트
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateRefundAccount($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  refund_name      = %s ";
        $query .= "\n           ,refund_bank_name = %s ";
        $query .= "\n           ,refund_ba_num    = %s ";
        $query .= "\n     WHERE  member_seqno     = %s ";

        $query = sprintf($query, $param["refund_name"]
                               , $param["refund_bank_name"]
                               , $param["refund_ba_num"]
                               , $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief 환불계좌정보 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function selectMemberVirtBaInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberInfoDAO.inc - 환불계좌정보 검색 query */ ";
        $query .= "\n        A.refund_name ";
        $query .= "\n       ,A.refund_bank_name ";
        $query .= "\n       ,A.refund_ba_num ";
        $query .= "\n       ,B.ba_num ";
        $query .= "\n       ,B.use_yn ";
        $query .= "\n       ,B.bank_name ";
        $query .= "\n       ,B.depo_name ";
        $query .= "\n  FROM  member AS A";
        $query .= "\n  INNER JOIN  virt_ba_admin AS B ON A.member_seqno = B.member_seqno";
        $query .= "\n WHERE  A.member_seqno = " . $param["member_seqno"];
        $query .= "\n AND B.use_yn = 'Y' ";

        return $conn->Execute($query);

    }

    /**
     * @brief 환불가능금액 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function selectRefundableAmount($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* MemberInfoDAO.inc - 환불가능금액 검색 query */ ";
        $query .= "\n        A.prepay_price_money ";
        $query .= "\n  FROM  member AS A";
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);

    }

    /**
     * @brief 환불 요청금액 입력
     *
     * @param $conn  = connection identifier
     *
     */
    function insertRefundAmount($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    INSERT INTO member_refund_history (";
        $query .= "\n     req_date";
        $query .= "\n    ,req_price";
        $query .= "\n    ,req_cont";
        $query .= "\n    ,refund_yn";
        $query .= "\n    ,member_seqno";
        $query .= "\n    ) VALUES (";
        $query .= "\n     now() ";
        $query .= "\n    ,%s ";
        $query .= "\n    ,%s ";
        $query .= "\n    ,'N' ";
        $query .= "\n    ,%s ";
        $query .= "\n    ) ";

        $query = sprintf($query, $param["refund_amount"]
                               , $param["refund_cont"]
                                , $_SESSION["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief 환불내역 조회
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectRefundReq($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        $query  = "\n   SELECT /* MemberInfoDAO.inc - selectRefundReq */";
        $query .= "\n          SQL_CALC_FOUND_ROWS";
        $query .= "\n          A.req_date";
        $query .= "\n         ,A.req_price";
        $query .= "\n         ,A.req_cont";
        $query .= "\n         ,A.refund_yn";
        $query .= "\n         ,A.refund_date";
        $query .= "\n    FROM  member_refund_history AS A";
        $query .= "\n   WHERE  A.member_seqno = " . $param["member_seqno"];
        $query .= "\nORDER BY  A.member_refund_history_seqno DESC";
        $query .= "\n    LIMIT " . $s_num . ", " . $list_num;

        return $conn->Execute($query);
    }
}
?>
