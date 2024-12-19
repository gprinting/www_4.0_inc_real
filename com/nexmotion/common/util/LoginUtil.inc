<?php
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/com/nexmotion/job/front/common/FrontCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/job/front/mypage/MemberDlvrDAO.inc");

class LoginUtil {
    private $conn;
    private $dao;
    private $dlvr_dao;
    private $res;
    private $dvs;
    private $seqno;
    private $state_arr;

    private $name;
    private $email;
    private $join_path;

    public $err_msg;

    function __construct($param) {
        $connectionPool = new ConnectionPool();
        $this->conn = $connectionPool->getPooledConnection();
        $this->dao  = new FrontCommonDAO();
        $this->dlvr_dao = new MemberDlvrDAO();

        $this->res = $param["res"];
        $this->dvs = $param["dvs"];
    }

    // 로그인 공통
    public function login(): bool {
        $funcName = $this->dvs;

        $ret = $this->$funcName();

        if (is_int($ret) && $ret < 0) {
            return true;
        }

        if (is_bool($ret) && !$ret) {
            $err_msg = "로그인 실패";
            $this->conn->Close();
            return false;
        }

        $this->makeSession();

        $this->conn->Close();
        return true;
    }

    // 이메일 로그인
    private function email(): bool {
        $conn = $this->conn;
        $dao  = $this->dao;
        $res  = $this->res;

        $name  = $res["member_name"];
        $email = $res["id"];            // 180102 아이디 컬럼 추가로 변경(기존 email)

        $param = [];
        $param["mail"]      = $email;
        $param["join_path"] = "Normal";

        $this->name      = $name;
        $this->email     = $email;
        $this->join_path = $param["join_path"];

        if (!$this->isMember($param)) {
            // 에러처리 따로 필요함
            echo "오류입니다.";
            return false;
        }
 
        return true;
    }

    // 네이버 로그인
    private function naver() {
        $conn = $this->conn;
        $dao  = $this->dao;

        $res   = json_decode($this->res, true);
        $name  = urldecode($res["response"]["name"]);
        $email = $res["response"]["email"];
        
        $param = [];
        $param["mail"] = $email;
        $param["name"] = $name;
        $param["join_path"] = "Naver"; 

        $this->email     = $email;
        $this->name      = $name;
        $this->join_path = $param["join_path"];

        $check = $this->isSubMember($param);
        
        // 기 연동된 계정
        if (!empty($check)) {
            unset($param);
            $param["seqno"] = $check;
            $result = $dao->selectMember($conn, $param); 

            $this->res = $result->fields;
            $this->dvs = "email";

            if ($this->login()) {
                return -1;
            }

        } else {
            if (!$this->isMember($param)) {
                // 사용자 데이터가 없으므로 새로 등록한다.
                $param["withdraw_yn"]  = "N"; 
                $param["grade"]        = "20"; 
                
                return $this->insertMember($param);
            }
        }

        return true;
    }

    // 카카오 로그인
    private function kakao() {
        $conn = $this->conn;
        $dao  = $this->dao;
        $res  = $this->res;

        // 카카오의 경우 실명이 아닌 닉네임을 전달받으므로 닉네임을 입력
        // 하지만 변수 통일을 위해 편의상 $name으로 칭하였다.
        $param = [];
        $param["mail"] = $res["email"];
        $param["name"] = $res["name"];
        $param["join_path"] = "Kakao"; 

        $this->email     = $res["email"];
        $this->name      = $res["name"];
        $this->join_path = $param["join_path"];

        $check = $this->isSubMember($param);

        // 기 연동된 계정
        if (!empty($check)) {
            unset($param);
            $param["seqno"] = $check;
            $result = $dao->selectMember($conn, $param); 

            $this->res = $result->fields;
            $this->dvs = "email";

            if ($this->login()) {
                return -1;
            }

        } else {
            if (!$this->isMember($param)) {
                // 사용자 데이터가 없으므로 새로 등록한다.
                $param["withdraw_yn"]  = "N"; 
                $param["grade"]        = "20"; 
                
                return $this->insertMember($param);
            }
        }

        return true;
    }

    // 페이스북 로그인
    private function fb() {
        $conn = $this->conn;
        $dao  = $this->dao;
        $res  = $this->res;
        
        $param = [];
        $param["mail"] = $res["email"];
        $param["name"] = $res["name"];
        $param["join_path"] = "Facebook"; 

        $this->email     = $res["email"];
        $this->name      = $res["name"];
        $this->join_path = $param["join_path"];
        
        if ($this->isSubMember($param)) {
            echo $check;
        } else {
            if (!$this->isMember($param)) {
                // 사용자 데이터가 없으므로 새로 등록한다.
                $param["withdraw_yn"]  = "N"; 
                $param["grade"]        = "20"; 
                
                return $this->insertMember($param);
            }
        }

        return true;
    }

    // 구글 로그인
    private function google() {
        $conn = $this->conn;
        $dao  = $this->dao;

        $res = json_decode($this->res, true);

        $email = $res["email"];
        $name  = $res["name"];
        
        $param = [];
        $param["mail"] = $email;
        $param["name"] = $name;
        $param["join_path"] = "Google"; 

        $this->email     = $email;
        $this->name      = $name;
        $this->join_path = $param["join_path"];

        $check = $this->isSubMember($param);

        // 기 연동된 계정
        if (!empty($check)) {
            unset($param);
            $param["seqno"] = $check;
            $result = $dao->selectMember($conn, $param); 

            $this->res = $result->fields;
            $this->dvs = "email";

            if ($this->login()) {
                return -1;
            }

        } else {
            if (!$this->isMember($param)) {

                // 사용자 데이터가 없으므로 새로 등록한다.
                $param["withdraw_yn"]  = "N"; 
                $param["grade"]        = "20"; 
                
                return $this->insertMember($param);
            }
        }

        return true;
    }

    // 회원정보 검색
    private function isMember($param): bool {
        $conn = $this->conn;
        $dao  = $this->dao;

        $check = $dao->selectSocialAcc($conn, $param);

        if (empty($check)) {
            return false;
        }

        return true;
    }

    // 서브회원정보 검색
    private function isSubMember($param): string {
        $conn = $this->conn;
        $dao  = $this->dao;

        $check = $dao->selectSocialAccSub($conn, $param);

        if (empty($check)) {
            $check = "";
        }

        return $check;
    }

    private function insertMember($param): bool {
        $conn = $this->conn;
        $dao  = $this->dao;

        $conn->StartTrans();

        $mem_ins = $dao->insertSocialAcc($conn, $param);
    
        if (!$mem_ins) {
            $this->err_msg = "입력에 실패했습니다.";
            $conn->FailTrans();
            $conn->RollbackTrans();
            return false;
        } else {
            $conn->CompleteTrans();

            return true;
        }
    }

    // 상세정보 불러오기
    private function searchMemberInfo() {
        $conn = $this->conn; 
        $dao  = $this->dao; 

        $param = [];
        $param["name"]      = $this->name;
        $param["mail"]      = $this->email;
        $param["join_path"] = $this->join_path;

        $info = $dao->selectMember($conn, $param);

        if (empty($info)) {
            $this->err_msg = "정보가 없습니다.";
            return false;
        }

        return $info->fields;
    }

    // 기본 배송지 정보
    private function searchBasicDlvr() {
        $conn = $this->conn;
        $dao  = $this->dlvr_dao;

        $param = [];
        $param["seqno"] = $this->seqno;

        $rs = $dao->selectBasicDlvr($conn, $param);

        return $rs->fields;
    }

    // 상품리스트 검색
    private function searchPrdtList($order_state) {
        $conn = $this->conn;
        $dao  = $this->dao;

        $param = [];
        $param["seqno"] = $this->seqno;
        $param["order_state"] = $order_state;

        $rs = $dao->selectOrderListByState($conn, $param);
        $count = $dao->selectFoundRows($conn);

        $prdt_arr = [];
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $prdt_arr[$fields["title"]] = $fields["title"];

            $rs->MoveNext();
        }

        return [
             "count" => $count
            ,"prdt"  => $prdt_arr
        ];
    }

    // 세션 설정
    private function makeSession() {
        if ($this->dvs === "email") {
            $member_rs = $this->res;
        } else {
            $member_rs = $this->searchMemberInfo();

            if (!$member_rs) {
                return false;
            }
        }


        if (empty($member_rs["group_id"])) {
            $org_member_seqno = $member_rs["member_seqno"];
        } else {
            $org_member_seqno = $member_rs["group_id"];
        }

        $this->seqno = $org_member_seqno;
        // 가상계좌 검색
        $virt_ba_info = $this->dao->selectMemberVirtBaInfo($this->conn, [
            "member_seqno" => $org_member_seqno
        ])->fields;

        $state_arr = $this->makeStateArr();
        $grade_arr = $this->makeGradeArr();

        // 기본배송지
        $basic_dlvr_rs = $this->searchBasicDlvr();
        // 배송중 상품
        $dlvr_prdt = $this->searchPrdtList($state_arr["배송중"]);
        // 장바구니 상품
        $cart_prdt = $this->searchPrdtList($state_arr["주문대기"]);

        // 세션에 값 저장
        $_SESSION["id"]                 = $this->email;
        $_SESSION["name"]               = $this->name;
        $_SESSION["join_path"]          = $this->join_path;
        $_SESSION["channel"]            = $member_rs["sell_channel"];


        $_SESSION["org_member_seqno"]   = $org_member_seqno;
        $_SESSION["member_seqno"]       = $member_rs["member_seqno"];
        $_SESSION["email"]              = $member_rs["mail"];
        $_SESSION["grade"]              = $grade_arr[$member_rs["grade"]];
        $_SESSION["level"]              = $member_rs["grade"];
        $_SESSION["join_path"]          = $member_rs["join_path"];
        $_SESSION["prepay_price_card"]  = $member_rs["prepay_price_card"];
        $_SESSION["prepay_price_money"] = $member_rs["prepay_price_money"];
        $_SESSION["member_dvs"]         = $member_rs["member_dvs"];
        $_SESSION["group_id"]           = $member_rs["group_id"];
        $_SESSION["group_name"]         = $member_rs["group_name"];
        $_SESSION["onefile_etprs_yn"]   = $member_rs["onefile_etprs_yn"];
        $_SESSION["card_pay_yn"]        = $member_rs["card_pay_yn"];
        $_SESSION["card_charge_yn"]     = $member_rs["card_charge_yn"];
        $_SESSION["cell_num"]           = $member_rs["cell_num"];
        $_SESSION["addr"]               = $member_rs["addr"];
        $_SESSION["zipcode"]            = $member_rs["zipcode"];         // 이상민 추가(24.02.12.)
        $_SESSION["addr_detail"]        = $member_rs["addr_detail"];    
        $_SESSION["tel_num"]            = $member_rs["tel_num"];         // 이상민 추가(24.02.12.)
        $_SESSION["direct_dlvr_yn"]     = $member_rs["direct_dlvr_yn"];
        $_SESSION["A_board_yn"]         = $member_rs["A_board_yn"];
        $_SESSION["dlvr_add_info"]      = $member_rs["dlvr_add_info"];
        $_SESSION["refund_ba_num"]      = $member_rs["refund_ba_num"];
        $_SESSION["refund_bank_name"]   = $member_rs["refund_bank_name"];
        $_SESSION["refund_name"]        = $member_rs["refund_name"];
        $_SESSION["mailing_yn"]         = $member_rs["mailing_yn"];
        $_SESSION["sms_yn"]             = $member_rs["sms_yn"];
        $_SESSION["sell_site"]          = "GPT";
        $_SESSION["state_arr"]          = $state_arr;
        $_SESSION["grade_arr"]          = $grade_arr;
        // 가상계좌
        $_SESSION["ba_name"]            = $virt_ba_info["bank_name"];
        $_SESSION["ba_num"]             = $virt_ba_info["ba_num"];
        // 기본배송지
        $_SESSION["basic_addr"]         = $basic_dlvr_rs["addr"];
        $_SESSION["basic_addr_detail"]  = $basic_dlvr_rs["addr_detail"];
        // 배송정보 주문한 상품
        $_SESSION["dlvr_prdt_count"]    = $dlvr_prdt["count"];
        $_SESSION["dlvr_prdt"]          = $dlvr_prdt["prdt"];
        // 장바구니
        $_SESSION["cart_prdt_count"]    = $cart_prdt["count"];
        $_SESSION["cart_prdt"]          = $cart_prdt["prdt"];
        // 세션유지용 로그인 시간
        $_SESSION["login_time"]         = time();
    }

    // 주문상태값 저장
    private function makeStateArr() {
        $conn = $this->conn;
        $dao  = $this->dao;

        $state_arr = [];
        $rs  = $dao->selectStateAdmin($conn);
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $state_arr[$fields["front_state_name"]] = $fields["state_code"];

            $rs->MoveNext();
        }
        return $state_arr;
    }

    // 주문상태값 저장
    private function makeGradeArr() {
        $conn = $this->conn;
        $dao  = $this->dao;

        $state_arr = [];
        $rs  = $dao->selectGradeSalePolicy($conn);
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $state_arr[$fields["grade"]] = $fields["front_grade_name"];

            $rs->MoveNext();
        }
        return $state_arr;
    }

    // 마지막 로그인 시간 변경
    private function updateLastLogin() {
        $conn = $this->conn;
        $dao  = $this->dao;

        $ret  = $this->searchInfo();

        $param = [];
        $param["member_seqno"] = $ret["member_seqno"];
    
        //USER 로그인시 로그인 시간 변경
        if (strpos($_SERVER["HTTP_REFERER"],"erp.yesprinting.co.kr") === false) { 
            $dao->updateMemberFinalLoginDate($conn, $param);
        }
    }

    // 페이지 리다이렉팅
    public function redirPage() {
        return "<script>window.opener.postMessage('success');self.close();</script>";
    }
}
