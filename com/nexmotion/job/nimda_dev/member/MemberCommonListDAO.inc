<?php
include_once(INC_PATH . "/com/nexmotion/job/nimda_dev/common/NimdaCommonDAO.inc");

class MemberCommonListDAO extends NimdaCommonDAO {
    /**
     * @brief 회원통합리스트 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectMemberList($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원통합리스트 검색 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         member_seqno";
        $query .= "\n        ,member_name";
        $query .= "\n        ,tel_num";
        $query .= "\n        ,cell_num";
        $query .= "\n        ,grade";
        $query .= "\n        ,first_join_date";
        $query .= "\n        ,final_order_date";
        $query .= "\n        ,final_login_date";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  group_id IS NULL";
        if ($this->blankParameterCheck($param ,"member_name")) {
            $member_name = substr($param["member_name"], 1, -1);
            $query .= "\n    AND  member_name LIKE '%%";
            $query .= $member_name . "%%'";
        }
        if ($this->blankParameterCheck($param ,"first_join_date")) {
            $query .= "\n    AND  first_join_date BETWEEN ";
            $query .= $param["first_join_date"]["from"];
            $query .= " AND ";
            $query .= $param["first_join_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"final_order_date")) {
            $query .= "\n    AND  final_order_date BETWEEN %s";
            $query .= $param["final_order_date"]["from"];
            $query .= " AND ";
            $query .= $param["final_order_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"final_login_date")) {
            $query .= "\n    AND  final_login_date BETWEEN %s";
            $query .= $param["final_login_date"]["from"];
            $query .= " AND ";
            $query .= $param["final_login_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"sell_channel")) {
            $query .= "\n    AND  sell_channel = ";
            $query .= $param["sell_channel"];
        }
        if ($this->blankParameterCheck($param ,"grade")) {
            $query .= "\n    AND  grade = ";
            $query .= $param["grade"];
        }
        $query .= "\n  ORDER  BY member_seqno DESC";
        $query .= "\n  LIMIT  %s, %s";

        $query  = sprintf($query, substr($param["page"], 1, -1)
                                , substr($param["page_block"], 1, -1));

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 선택한 회원 상세정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectMemberDetailInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 상세정보 검색 */";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.mail";
        $query .= "\n        ,A.grade";
        $query .= "\n        ,A.join_path";
        $query .= "\n        ,A.member_dvs";
        $query .= "\n        ,A.onefile_etprs_yn";
        $query .= "\n        ,A.card_pay_yn";
        $query .= "\n        ,A.card_charge_yn";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.zipcode";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";
        $query .= "\n        ,A.refund_bank_name";
        $query .= "\n        ,A.refund_name";
        $query .= "\n        ,A.refund_ba_num";
        $query .= "\n        ,A.mailing_yn";
        $query .= "\n        ,A.sms_yn";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        return $conn->Execute($query, [$param["member_seqno"]])->fields;
    }

    /**
     * @brief 선택한 회원 가상계좌정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectMemberVirtBaInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 가상계좌정보 검색 */";
        $query .= "\n         A.ba_num";
        $query .= "\n        ,A.bank_name";
        $query .= "\n   FROM  virt_ba_admin AS A"; // TODO 수정필요
        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.use_yn = 'Y'";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        return $conn->Execute($query, [$param["member_seqno"]])->fields;
    }

    /**
     * @brief 소셜로그인 연동 확인
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectSocLinkInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  /* [MemberCommonListDAO.inc] 소셜로그인 연동 확인 */";
        $query .= "\n        mail ";
        $query .= "\n       ,join_path ";
        $query .= "\n  FROM  member_sub_id";
        $query .= "\n WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원 성향정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectDetailInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 성향정보 검색 */";
        $query .= "\n         A.occu1";
        $query .= "\n        ,A.occu2";
        $query .= "\n        ,A.plural_deal_site_name1";
        $query .= "\n        ,A.plural_deal_site_name2";
        $query .= "\n        ,A.occu_detail";
        $query .= "\n        ,A.interest_field1";
        $query .= "\n        ,A.interest_field2";
        $query .= "\n        ,A.interest_field_detail";
        $query .= "\n        ,A.design_outsource_yn";
        $query .= "\n        ,A.produce_outsource_yn";
        $query .= "\n        ,A.use_pro";
        $query .= "\n        ,A.use_opersys";
        $query .= "\n        ,A.add_interest_items";
        $query .= "\n        ,A.interest_prior";
        $query .= "\n        ,A.plural_deal_yn";
        $query .= "\n        ,A.plural_deal_site_detail1";
        $query .= "\n        ,A.plural_deal_site_detail2";
        $query .= "\n        ,A.recomm_id";
        $query .= "\n        ,A.recomm_id_detail";
        $query .= "\n        ,A.memo";
        $query .= "\n   FROM  member_detail_info AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 사업자정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectCorpInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 사업자정보 검색 */";
        $query .= "\n         A.corp_name";
        $query .= "\n        ,A.repre_name";
        $query .= "\n        ,A.crn";
        $query .= "\n        ,A.bc";
        $query .= "\n        ,A.tob";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";
        $query .= "\n        ,A.zipcode";
        $query .= "\n   FROM  licensee_info AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 관심상품 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectPrdtInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 관심상품 검색 */";
        $query .= "\n         A.interest_1";
        $query .= "\n        ,A.interest_2";
        $query .= "\n        ,A.interest_3";
        $query .= "\n        ,A.interest_4";
        $query .= "\n        ,A.interest_5";
        $query .= "\n        ,A.interest_6";
        $query .= "\n        ,A.interest_7";
        $query .= "\n        ,A.interest_8";
        $query .= "\n        ,A.interest_9";
        $query .= "\n        ,A.interest_10";
        $query .= "\n        ,A.interest_11";
        $query .= "\n        ,A.interest_12";
        $query .= "\n   FROM  member_interest_prdt AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 관심디자인 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectDesignInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 관심상품 검색 */";
        $query .= "\n         A.interest_1";
        $query .= "\n        ,A.interest_2";
        $query .= "\n        ,A.interest_3";
        $query .= "\n        ,A.interest_4";
        $query .= "\n        ,A.interest_5";
        $query .= "\n        ,A.interest_6";
        $query .= "\n   FROM  member_interest_design AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 관심이벤트 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectEventInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 관심상품 검색 */";
        $query .= "\n         A.interest_1";
        $query .= "\n        ,A.interest_2";
        $query .= "\n        ,A.interest_3";
        $query .= "\n        ,A.interest_4";
        $query .= "\n        ,A.interest_5";
        $query .= "\n   FROM  member_interest_event AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }
    
    /**
     * @brief 선택한 회원 관심요구사항 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectNeedsInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 관심상품 검색 */";
        $query .= "\n         A.interest_1";
        $query .= "\n        ,A.interest_2";
        $query .= "\n        ,A.interest_3";
        $query .= "\n        ,A.interest_4";
        $query .= "\n        ,A.interest_5";
        $query .= "\n        ,A.interest_6";
        $query .= "\n        ,A.interest_7";
        $query .= "\n        ,A.interest_8";
        $query .= "\n        ,A.interest_9";
        $query .= "\n        ,A.interest_10";
        $query .= "\n   FROM  member_interest_needs AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 주문담당자 정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectOrderManInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 주문담당자 정보 검색 */";
        $query .= "\n         SQL_CALC_FOUND_ROWS";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,A.mail";
        $query .= "\n        ,A.member_seqno";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  A.group_id = %s";
        $query .= "\n    AND  A.withdraw_yn = 'N'";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

    /**
     * @brief 선택한 회원 회계담당자 정보 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectAccountingManInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [MemberCommonListDAO.inc] 회원 회계담당자 정보 검색 */";
        $query .= "\n         SQL_CALC_FOUND_ROWS";
        $query .= "\n         A.name";
        $query .= "\n        ,A.posi";
        $query .= "\n        ,A.mail";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,A.accting_mng_seqno";
        $query .= "\n   FROM  accting_mng AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["member_seqno"];

        return $conn->Execute($query, $bind);
    }

}
