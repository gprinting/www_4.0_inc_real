<?
include_once(INC_PATH . '/common_dao/CommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/MakeCommonHtml.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/common/FrontCommonHtml.inc');
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc");

/*! 공통 DAO Class */
class FrontCommonDAO extends CommonDAO { 
    /**
     * @brief 카테고리 검색
     *
     * @param $conn         = connection identifier
     * @param $sel_sortcode = html 선택으로 표시할 분류코드
     * @param $sortcode     = 검색조건 분류코드
     *
     * @return 검색결과
     */
    function selectCateHtml($conn, $dvs, $sel_sortcode, $sortcode = null, $is_mobile = false) {
        if($sel_sortcode == "005001001" || $sel_sortcode == "005001002" || $sel_sortcode == "005001003" ) {
            $rs = $this->selectCateTopDown2($conn, $sel_sortcode, $sortcode);

        }else{
        $rs = $this->selectCateTopDown($conn, $sel_sortcode, $sortcode);

        }
        if ($rs->RecordCount() === 1) {
            // 카테고리 값이 1개인 경우 selectbox 처리 안함
            $cate_name = $rs->fields["cate_name"];
            $ret = <<<html
                {$cate_name}
                <select id="{$dvs}_cate_sortcode" name="{$dvs}_cate_sortcode" style="display:none;">
                    <option value="{$sel_sortcode}">{$cate_name}</option>
                </select>
html;
        } else {
            $arr = [];
            $arr["val"] = "sortcode";
            $arr["dvs"] = "cate_name";
            $arr["sel"] = $sel_sortcode;

            $opt = makeOptionHtmlFront($rs, $arr);

            $class = "_relatedSummary _productName";
            if ($is_mobile) {
                $class = "basic_input _summary _cate";
            }


            $ret = <<<html
                            <select onchange="moveProduct(this.value);" class="{$class}" id="{$dvs}_cate_sortcode" name="{$dvs}_cate_sortcode">
                                {$opt}
                            </select>
html;
        }
        return $ret;
    }

    function startsWith($haystack, $needle) {
        // search backwards starting from haystack length characters from the end
        return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== false;
    }

    function selectCateTopDown($conn, $sel_sortcode, $sortcode = null) {
        $param = [];
        $param["col"]   = "sortcode, cate_name";
        $param["table"] = "cate";
        if ($sortcode === null) {
            $param["where"]["cate_level"] = "1";
        } else {
            $param["where"]["high_sortcode"] = $sortcode;
            $param["where"]["use_yn"] = 'Y';
        }

        $param["order"] = "seq, sortcode, cate_name";
        $rs = $this->selectData($conn, $param);
        return $rs;
    }

    function selectCateTopDown2($conn, $sel_sortcode, $sortcode = null) {
        $param = [];
        $param["col"]   = "sortcode, cate_name";
        $param["table"] = "cate";
        if ($sortcode === null) {
            $param["where"]["cate_level"] = "1";
        } else {
            $param["where"]["sortcode"] = $sel_sortcode;
            $param["where"]["use_yn"] = 'Y';
        }

        $param["order"] = "seq, sortcode, cate_name";
        $rs = $this->selectData($conn, $param);
        return $rs;
    }

    function selectOrderNum3($conn, $param) {
        $param = [];
        $param["col"]   = "sortcode, cate_name";
        $param["table"] = "cate";
        if ($sortcode === null) {
            $param["where"]["cate_level"] = "1";
        } else {
            $param["where"]["high_sortcode"] = $sortcode;
            $param["where"]["use_yn"] = 'Y';
        }

        $param["order"] = "seq, sortcode, cate_name";

        $rs = $this->selectData($conn, $param);

        return $rs;
    }

    /*
     * @brief 지번 주소 Select 
     * @param $conn : DB Connection
     * @param $param["val"] : 지번 검색어

     * @param $param["area"] : 지역
     * @return : resultSet 
     */ 
    function selectJibunZip($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $area = substr($param["area"], 1, -1); 
        $val = substr($param["val"], 1, -1); 

        $query  = "\n    SELECT  zipcode";
        $query .= "\n           ,sido";
        $query .= "\n           ,gugun";
        $query .= "\n           ,eup";
        $query .= "\n           ,dong";
        $query .= "\n           ,bldg";
        $query .= "\n           ,jibun_bonbun";
        $query .= "\n           ,jibun_bubun";
        $query .= "\n           ,bldg";
        $query .= "\n           ,ri";
        $query .= "\n      FROM  " . $area . "_zipcode";
        $query .= "\n     WHERE  (dong LIKE '%" . $val . "%'";
        $query .= "\n        OR   eup LIKE '%" . $val . "%'";
        $query .= "\n        OR   ri LIKE '%" . $val . "%')";

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * @brief 도로명 주소 Select 
     * @param $conn : DB Connection
     * @param $param["val"] : 지번 검색어
     * @param $param["area"] : 지역
     * @return : resultSet 
     */ 
    function selectDoroZip($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $area = substr($param["area"], 1, -1); 
        $val = substr($param["val"], 1, -1); 

        $query  = "\n    SELECT  zipcode";
        $query .= "\n           ,sido";
        $query .= "\n           ,gugun";
        $query .= "\n           ,doro";
        $query .= "\n           ,bldg";
        $query .= "\n           ,bldg_bonbun";
        $query .= "\n           ,bldg_bubun";
        $query .= "\n      FROM  " . $area . "_zipcode";
        $query .= "\n     WHERE  doro LIKE '%" . $val .  "%'";

        $result = $conn->Execute($query);

        return $result;
    }

    /**
     * @brief 아이디와 비밀번호로 회원 정보 검색
     *
     * @detail $param["id"] = 회원 아이디
     * $param["seqno"] = 회원 일련번호
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.member_seqno ";      /* 회원일련번호 */
        $query .= "\n           ,A.member_name ";       /* 회원명 */
        $query .= "\n           ,A.id ";                /* 회원아이디(메일형식) */
        $query .= "\n           ,A.mail ";              /* 회원 이메일 */
        $query .= "\n           ,A.grade ";             /* 등급 */
        $query .= "\n           ,A.passwd ";            /* 비밀번호 */
        $query .= "\n           ,A.join_path ";         /* 가입경로 */
        $query .= "\n           ,A.cumul_sales_price "; /* 누적매출금액 */
        $query .= "\n           ,A.prepay_price_card "; /* 선입금금액(카드) */
        $query .= "\n           ,A.prepay_price_money ";/* 선입금금액(현금) */
        $query .= "\n           ,A.order_lack_price ";  /* 주문부족금액 */
        $query .= "\n           ,A.first_join_date ";   /* 최초가입일자 */
        $query .= "\n           ,A.final_order_date ";  /* 최종주문일자 */
        $query .= "\n           ,A.final_login_date ";  /* 최종로그인일자 */
        $query .= "\n           ,A.final_modi_date ";   /* 최종수정일자 */
        $query .= "\n           ,A.member_dvs";         /* 회원구분 */
        $query .= "\n           ,A.group_id ";          /* 그룹아이디 */
        $query .= "\n           ,A.group_name ";        /* 그룹명 */
        $query .= "\n           ,A.onefile_etprs_yn ";  /* 원파일업체여부 */
        $query .= "\n           ,A.card_pay_yn ";       /* 카드결제여부 */
        $query .= "\n           ,A.card_charge_yn ";    /* 카드충전여부 */
        $query .= "\n           ,A.cell_num ";          /* 핸드폰번호 */
        $query .= "\n           ,A.zipcode ";           /* 우편번호 */
        $query .= "\n           ,A.tel_num ";           /* 전화번호 */
        $query .= "\n           ,A.addr ";              /* 주소 */
        $query .= "\n           ,A.fax_num ";           /* 팩스번호 */
        $query .= "\n           ,A.addr_detail ";       /* 상세주소 */
        $query .= "\n           ,A.direct_dlvr_yn ";    /* 직배송여부 */
        $query .= "\n           ,A.A_board_yn ";        /* A판여부 */
        $query .= "\n           ,A.dlvr_add_info";      /* 배송추가정보 */
        $query .= "\n           ,A.noted_items";        /* 참고사항 */
        $query .= "\n           ,A.refund_ba_num";      /* 환불계좌번호 */
        $query .= "\n           ,A.refund_bank_name";   /* 환불은행이름 */
        $query .= "\n           ,A.refund_name";        /* 환불이름 */
        $query .= "\n           ,A.mailing_yn";         /* 메일링여부 */
        $query .= "\n           ,A.sell_channel";       /* 채널구분 */
        $query .= "\n           ,A.sms_yn";             /* sms여부 */

        $query .= "\n      FROM  member  AS A ";
        $query .= "\n     WHERE  1 = 1";
        $query .= "\n       AND  A.withdraw_yn = 'N'";
        if ($this->blankParameterCheck($param, "id")) {
            $query .= "\n      AND  A.id = " . $param["id"];
        }
        if ($this->blankParameterCheck($param, "seqno")) {
            $query .= "\n      AND  A.member_seqno = " . $param["seqno"];
        }
        if ($this->blankParameterCheck($param, "name")) {
            $query .= "\n    AND  A.member_name = ";
            $query .= $param["name"];
        }
        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.mail = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "pw")) {
            $query .= "\n    AND  A.passwd = PASSWORD(";
            $query .= $param["pw"];
            $query .= ")";
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            $query .= "\n    AND  A.join_path = ";
            $query .= $param["join_path"];
        }
        if ($this->blankParameterCheck($param, "sell_channel")) {
            if($param["sell_channel"] == "GP") {
                $query .= "\n    AND  (A.sell_channel = '' OR A.sell_channel IS NULL OR A.sell_channel = 'GP') ";
            } else {
                $query .= "\n    AND  A.sell_channel = ";
                $query .= $param["sell_channel"];
            }
        }

        return $conn->Execute($query);

    }

    /**
     * @brief 기업회원 정보 검색
     *
     * @detail $param["id"] = 회원 아이디
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOfficeMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.crn ";      /* 사업자등록번호 */
        $query .= "\n      FROM  licensee_info AS A ";
        $query .= "\n     WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "seqno")) {
            $query .= "\n      AND  A.member_seqno = " . $param["seqno"];
        }

        return $conn->Execute($query);

    }



    /**
     * @brief 쿠폰 매수 검색
     *
     * @param $conn  = connection identifier
     * @param $Seqno = 회원 일련번호
     *
     * @return 쿠폰 매수
     */
    function selectMemberCpCount($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query .= "\nSELECT  COUNT(*) AS cp_count ";
        $query .= "\n  FROM  cp_use_history AS A ";
        $query .= "\n WHERE  member_seqno = %s";
        $query .= "\n   AND  A.use_yn = 'N'";

        $query  = sprintf($query, $seqno);

        $rs = $conn->Execute($query);

        return $rs->fields["cp_count"];
    }

    /**
     * @brief 주문 요약 배열 생성
     *
     * @detail $param["seqno"] = 회원일련번호
     * $param["date"] = 요약범위
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건파라미터
     *
     * @return 쿠폰 매수
     */
    function selectOrderSummary($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  B.dvs";
        $query .= "\n        ,count(1) AS state_count";

        $query .= "\n   FROM  order_common AS A";
        $query .= "\n        ,state_admin  AS B";

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.order_state = B.state_code";
        $query .= "\n    AND  A.order_regi_date BETWEEN %s AND %s";

        $query .= "\n  GROUP BY B.dvs";

        $query  = sprintf($query, $param["seqno"]
                                , $param["start_date"]
                                , $param["end_date"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문상태별 주문리스트
     *
     * @detail $param["seqno"] = 회원일련번호
     * $param["order_state"] = 주문상태
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건파라미터
     *
     * @return 리스트
     */
    function selectOrderListByState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n   SELECT  /* [FrontCommonDAO.inc] 사이드메뉴 주문 리스트 검색용 */";
        $query .= "\n           SQL_CALC_FOUND_ROWS";
        $query .= "\n           title";
        $query .= "\n          ,order_detail";
        $query .= "\n          ,order_common_seqno";

        $query .= "\n     FROM  order_common";

        $query .= "\n    WHERE  member_seqno = %s";
        $query .= "\n      AND  order_state  = %s";

        $query .= "\n ORDER BY  order_common_seqno DESC";
        $query .= "\n    LIMIT  5";
        
        $query  = sprintf($query, $conn->param($param["seqno"])
                                , $conn->param($param["order_state"]));

        $bind = [];
        $bind[] = $param["seqno"];
        $bind[] = $param["order_state"];

        return $conn->Execute($query, $bind);
    }

    function selectOrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n    SELECT  order_num";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_common_seqno = %s";
        $query  = sprintf($query, $param["order_common_seqno"]);

        $result = $conn->Execute($query);
        return $result->fields["order_num"];
    }

    /**
     * @brief 카테고리 종이 정보 검색
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 종이 맵핑코드
     * @param $col    = 상품종이에서 검색할 필드
     *
     * @return 종이 기준단위
     */
    function selectCatePaperInfo($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query .= "\n    SELECT  sort, name, dvs, color, basisweight";
        $query .= "\n      FROM  cate_paper";
        $query .= "\n     WHERE  mpcode = %s";

        $query  = sprintf($query, $conn->param($mpcode));

        $rs = $conn->Execute($query, [$mpcode]);

        return $rs->fields;
    }

    function selectOrderCommonSeqnoByOrderNum($conn, $order_num) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT  order_common_seqno ";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_num = '%s'";

        $query  = sprintf($query, $order_num);

        $rs = $conn->Execute($query);
        return $rs->fields["order_common_seqno"];
    }

    function selectOrderNumByOrderCommonSeqno($conn, $order_common_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT  order_num ";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_common_seqno = '%s'";

        $query  = sprintf($query, $order_common_seqno);

        $rs = $conn->Execute($query);
        return $rs->fields["order_num"];
    }

    /**
     * @brief 카테고리 인쇄도수정보 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     * @param &$price_info_arr = 가격검색용 정보저장 배열
     *
     * @return 면 구분별 html 배열
     */
    function selectCatePrintTmpt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["sortcode_m"] = substr($param["cate_sortcode"], 0, 6);

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.name";
        $query .= "\n        ,A.side_dvs";
        $query .= "\n        ,A.purp_dvs";
        $query .= "\n        ,B.affil";
        $query .= "\n        ,C.mpcode";

        $query .= "\n   FROM  prdt_print      AS A";
        $query .= "\n        ,prdt_print_info AS B";
        $query .= "\n        ,cate_print      AS C";

        $query .= "\n  WHERE  A.prdt_print_seqno = C.prdt_print_seqno";
        $query .= "\n    AND  A.print_name       = B.print_name";
        $query .= "\n    AND  A.purp_dvs         = B.purp_dvs";
        //$query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  C.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  C.mpcode        = " . $param["mpcode"];
        }
        if ($this->blankParameterCheck($param, "affil")) {
            $query .= "\n    AND  B.affil         = " . $param["affil"];
        }
        if ($this->blankParameterCheck($param, "purp_dvs")) {
            $query .= "\n    AND  A.purp_dvs      = " . $param["purp_dvs"];
        }
        $query .= "\n ORDER BY C.seq";
        if (empty($default_purp)) {
            $query .= ", A.purp_dvs DESC, C.seq DESC";
        }

        $query  = sprintf($query //, $param["sortcode_m"]
                                , $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고리 사이즈정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateSize($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $except_arr = ["cate_mpcode" => true];

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.name";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.work_wid_size";
        $query .= "\n        ,A.work_vert_size";
        $query .= "\n        ,A.cut_wid_size";
        $query .= "\n        ,A.cut_vert_size";
        $query .= "\n        ,A.tomson_wid_size";
        $query .= "\n        ,A.tomson_vert_size";
        $query .= "\n        ,A.design_wid_size";
        $query .= "\n        ,A.design_vert_size";
        $query .= "\n        ,A.affil";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  prdt_stan AS A";
        $query .= "\n        ,cate_stan AS B";

        $query .= "\n  WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "typ")) {
            $query .= "\n    AND  A.typ = ";
            $query .= $param["typ"];
        }

        if ($this->blankParameterCheck($param, "cate_mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["cate_mpcode"];
            $query .= ')';
        }
        $query .= "\n  ORDER  BY B.seq, A.name asc";

        $query  = sprintf($query, $param["cate_sortcode"]);
        $rs = $conn->Execute($query);
        return $rs;
    }
    

    /**
     * @brief 도수명과 인쇄용도로 카테고리 인쇄 맵핑코드 검색
     *
     * @detail $param["name"] = 카테고리 후공정 맵핑코드
     * $param["purp_dvs"] = 판매채널
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCatePrintMpcode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  C.mpcode";

        $query .= "\n   FROM  prdt_print      AS A";
        $query .= "\n        ,prdt_print_info AS B";
        $query .= "\n        ,cate_print      AS C";

        $query .= "\n  WHERE  A.prdt_print_seqno = C.prdt_print_seqno";
        $query .= "\n    AND  A.print_name       = B.print_name";
        $query .= "\n    AND  A.purp_dvs         = B.purp_dvs";
        $query .= "\n    AND  A.name             = %s";
        $query .= "\n    AND  A.purp_dvs         = %s";
        $query .= "\n    AND  C.cate_sortcode    = %s";
        if ($this->blankParameterCheck($param, "affil")) {
            $query .= "\n    AND  B.affil            = ";
            $query .= $param["affil"];
        }
        if ($this->blankParameterCheck($param, "side_dvs")) {
            $query .= "\n    AND  A.side_dvs         = ";
            $query .= $param["side_dvs"];
        }

        $query  = sprintf($query, $param["name"]
                                , $param["purp_dvs"]
                                , $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields["mpcode"];
    }

    /**
     * @brief 회원 마지막 로그인 시간 변경
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberFinalLoginDate($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["final_login_date"] = date("Y-m-d H:i:s");

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  final_login_date = %s ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["final_login_date"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원정보 비밀번호 변경
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberPw($conn, $param) {
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

        $query = sprintf($query, $param["passwd"],
                         $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 결제확인 팝업용 사용자 기본정보 검색
     *
     * @param $conn = connection identifier
     * @param $member_seqno = 사용자 일련번호
     *
     * @return 검색결과
     */
    function selectMemberInfo($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  A.mail";
        $query .= "\n        ,A.is_except";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,B.tel_num";
        $query .= "\n        ,B.cell_num";
        $query .= "\n        ,B.zipcode";
        $query .= "\n        ,B.addr";
        $query .= "\n        ,B.addr_detail";
        $query .= "\n   FROM  member AS A";
        $query .= "\n   LEFT JOIN member_dlvr AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  WHERE  1 = 1 ";
        $query .= "\n    AND  A.member_seqno = %s";
        $query .= "\n   ORDER BY B.basic_yn DESC LIMIT 1 ";

        $query  = sprintf($query, $member_seqno);
       // return $query;
        $rs = $conn->Execute($query);

        return $rs->fields;
    }


     /**
     * @brief 결제확인 팝업용 사용자 기본정보 검색
     *
     * @param $conn = connection identifier
     * @param $member_seqno = 사용자 일련번호
     *
     * @return 검색결과
     */
    function selectMemberInfo4($conn, $dvs, $member_seqno) {

        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //$member_seqno = $this->parameterEscape($conn, $member_seqno);
        if ($dvs == "COUNT") {
            $query  ="\nSELECT COUNT(*) AS cnt FROM ( SELECT b.* FROM ";
        } else if ($dvs == "SEQ") {
            $query  ="\n SELECT b.* FROM ";
        }
        $query .= "\n  (SELECT mb_seqno FROM member_point GROUP BY mb_seqno) AS a JOIN member AS b on a.mb_seqno = b.member_seqno";
        //$query .= "\n  WHERE  a.chk_yn = 'Y' ";
        if($member_seqno[member_seqno] != ""){
            $query .= "\n  AND  b.member_seqno = $member_seqno[member_seqno] ";
        }  

        if($member_seqno[sell_ch] != ""){
            $query .= "\n  AND  b.sell_channel = '$member_seqno[sell_ch]'";
        }  

        if($member_seqno[man_list] != ""){
            $query .= "\n  AND  b.chk_yn = '$member_seqno[man_list]'";
        }  

        if($member_seqno[state_list] == "y"){
            $query .= "\n  AND  b.con_man IS NOT NULL ";
        }else if($member_seqno[state_list] == "n"){
            $query .= "\n  AND  b.con_man IS NULL ";
        }             

        //$query .= "\n   GROUP BY member_seqno";
        $query .= "\n   ORDER BY member_seqno";
        
        if ($dvs == "COUNT") {
         $query .="\n ) as c";
        }else{
         $query .="";
        }
        
        $s_num = $member_seqno[s_num];
        $list_num = $member_seqno[list_num];

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        //echo $query;
        $rs = $conn->Execute($query);
        return $rs;
        
    }
    /**
     * @brief 결제확인 팝업용 사용자 기본정보 검색
     *
     * @param $conn = connection identifier
     * @param $member_seqno = 사용자 일련번호
     *
     * @return 검색결과
     */
    function selectCounselInfo($conn, $dvs, $member_seqno) {

        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //$member_seqno = $this->parameterEscape($conn, $member_seqno);
        if ($dvs == "COUNT") {
            $query  ="\nSELECT COUNT(*) AS cnt FROM ";
        } else if ($dvs == "SEQ") {
            $query  ="\n SELECT * FROM ";
        }
        $query .= "\n  con_list AS a JOIN member AS b  ON a.con_custom = b.member_seqno ";
        $query .= "\n  where con_man in('dpuser32','dpuser34','dpuser38','dpuser39','dpuser46')  ";
        if($member_seqno[member_seqno] != ""){
            $query .= "\n  AND  b.member_seqno = $member_seqno[member_seqno] ";
        }  

        if($member_seqno[sell_ch] != ""){
            $query .= "\n  AND  b.sell_channel = '$member_seqno[sell_ch]'";
        }  

        if($member_seqno[man_list] != ""){
            $query .= "\n  AND  con_man = '$member_seqno[man_list]'";
        } 

        if($member_seqno[date_from] != "" && $member_seqno[date_to] != "" ){
            $from = $member_seqno['date_from'];
            $to = $member_seqno['date_to'];
            $query .="\n  and con_date between '$from' and '$to' "; 

        }


        if($member_seqno[state_list] == "y"){
            $query .= "\n  AND  b.con_man IS NOT NULL ";
        }else if($member_seqno[state_list] == "n"){
            $query .= "\n  AND  b.con_man IS NULL ";
        }             

        //$query .= "\n   GROUP BY member_seqno";
        $query .= "\n  ORDER BY con_date desc";
        if ($dvs == "COUNT") {
        // $query .="\n ) as c";
        }else{
         $query .="";
        }
        
        $s_num = $member_seqno[s_num];
        $list_num = $member_seqno[list_num];

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
       // echo $query;
        $rs = $conn->Execute($query);
        return $rs;
        
    }


    /**
     * @brief 결제확인 팝업용 사용자 기본정보 검색
     *
     * @param $conn = connection identifier
     * @param $member_seqno = 사용자 일련번호
     *
     * @return 검색결과
     */
    function selectMemberConInfo($conn, $dvs, $member_seqno) {

        
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  ="\nSELECT  * ";
        }

        $query .= "\n   FROM  con_list";
        $query .= "\n  WHERE  con_custom = $member_seqno ";
        $query .= "\n   ORDER BY con_idx DESC";

        $rs = $conn->Execute($query);

        return $rs;
        //echo $query;
    }


    /**
     * @brief 회원_결제_내역 정보 입력
     *
     * @param $conn = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 쿼리 실행결과
     */
    function insertMemberPayHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_pay_history (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,deal_date";
        $query .= "\n     ,order_num";
        $query .= "\n     ,dvs";
        $query .= "\n     ,sell_price";
        $query .= "\n     ,sale_price";
        $query .= "\n     ,pay_price";
        $query .= "\n     ,card_pay_price";
        $query .= "\n     ,depo_price";
        $query .= "\n     ,card_depo_price";
        $query .= "\n     ,exist_prepay";
        $query .= "\n     ,prepay_bal";
        $query .= "\n     ,state";
        $query .= "\n     ,deal_num";
        $query .= "\n     ,order_cancel_yn";
        $query .= "\n     ,pay_year";
        $query .= "\n     ,pay_mon";
        $query .= "\n     ,prepay_use_yn";
        $query .= "\n     ,input_typ";
        $query .= "\n     ,cont";
        $query .= "\n     ,public_dvs";
        $query .= "\n     ,public_state";
        $query .= "\n     ,dvs_detail";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,now()";
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
        $query .= "\n     ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["order_num"]
                                , $param["dvs"]
                                , $param["sell_price"]
                                , $param["sale_price"]
                                , $param["pay_price"]
                                , $param["card_pay_price"]
                                , $param["depo_price"]
                                , $param["card_depo_price"]
                                , $param["exist_prepay"]
                                , $param["prepay_bal"]
                                , $param["state"]
                                , $param["deal_num"]
                                , $param["order_cancel_yn"]
                                , $param["pay_year"]
                                , $param["pay_mon"]
                                , $param["prepay_use_yn"]
                                , $param["input_typ"]
                                , $param["cont"]
                                , $param["public_dvs"]
                                , $param["public_state"]
                                , $param["dvs_detail"]);

        return $conn->Execute($query);
    }

    function insertMemberPrepayHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_prepay_history (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,deal_date";
        $query .= "\n     ,dvs";
        $query .= "\n     ,depo_price";
        $query .= "\n     ,exist_prepay";
        $query .= "\n     ,prepay_bal";
        $query .= "\n     ,deal_num";
        $query .= "\n     ,pay_year";
        $query .= "\n     ,pay_mon";
        $query .= "\n     ,input_typ";
        $query .= "\n     ,cont";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,now()";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
            , $param["dvs"]
            , $param["depo_price"]
            , $param["exist_prepay"]
            , $param["prepay_bal"]
            , $param["deal_num"]
            , $param["pay_year"]
            , $param["pay_mon"]
            , $param["input_typ"]
            , $param["cont"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원_결제_내역_카드 정보 입력
     *
     * @param $conn = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 쿼리 실행결과
     */
    function insertMemberPayHistoryCard($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_pay_history_card (";
        $query .= "\n      member_pay_history_seqno";
        $query .= "\n     ,card_cpn";
        $query .= "\n     ,aprvl_num";
        $query .= "\n     ,card_num";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_pay_history_seqno"]
                                , $param["card_cpn"]
                                , $param["aprvl_num"]
                                , $param["card_num"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원_결제_내역 집계 정보 검색
     *
     * @param $conn = connection identifier
     *
     * @return 검색 결과
     */
    function selectMemberDaySales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT  A.member_seqno";
        $query .= "\n          ,A.input_date";
        /*
        $query .= "\n          ,A.sales_price";
        $query .= "\n          ,A.sale_price";
        $query .= "\n          ,A.net_sales_price";
        $query .= "\n          ,A.card_net_sales_price";
        $query .= "\n          ,A.depo_price";
        $query .= "\n          ,A.card_depo_price";
        */
        $query .= "\n          ,A.period_end_oa";
        $query .= "\n          ,A.carryforward_oa";
        $query .= "\n     FROM  day_sales_stats AS A";
        $query .= "\n    WHERE  A.member_seqno = %s";
        $query .= "\n ORDER BY  A.input_date DESC";
        $query .= "\n    LIMIT  1";

        $query = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원_결제_내역 집계 정보 입력
     *
     * @param $conn = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 쿼리 실행결과
     */
    function insertMemberDaySales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO day_sales_stats (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,input_date";
        $query .= "\n     ,sales_price";
        $query .= "\n     ,sale_price";
        $query .= "\n     ,net_sales_price";
        $query .= "\n     ,card_net_sales_price";
        $query .= "\n     ,depo_price";
        $query .= "\n     ,card_depo_price";
        $query .= "\n     ,period_end_oa";
        $query .= "\n     ,carryforward_oa";
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
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_date"]
                                , $param["sales_price"]
                                , $param["sale_price"]
                                , $param["net_sales_price"]
                                , $param["card_net_sales_price"]
                                , $param["depo_price"]
                                , $param["card_depo_price"]
                                , $param["period_end_oa"]
                                , $param["carryforward_oa"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원_결제_내역 집계 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 입력값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMemberDaySales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  day_sales_stats as A";
        $query .= "\n    SET  A.member_seqno = A.member_seqno";
        if ($this->blankParameterCheck($param, "sales_price")) {
            $query .= "\n        ,A.sales_price  = IFNULL(A.sales_price, 0) + ";
            $query .= $param["sales_price"];
        }
        if ($this->blankParameterCheck($param, "sale_price")) {
            $query .= "\n        ,A.sale_price = IFNULL(A.sale_price, 0) + ";
            $query .= $param["sale_price"];
        }
        if ($this->blankParameterCheck($param, "net_sales_price")) {
            $query .= "\n        ,A.net_sales_price = IFNULL(A.net_sales_price, 0) + ";
            $query .= $param["net_sales_price"];
        }
        if ($this->blankParameterCheck($param, "card_net_sales_price")) {
            $query .= "\n        ,A.card_net_sales_price = IFNULL(A.card_net_sales_price, 0) + ";
            $query .= $param["card_net_sales_price"];
        }

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.input_date   = %s";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_date"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 카테고리별 집계 정보 검색
     *
     * @param $conn = connection identifier
     *
     * @return 검색 결과
     */
    function selectMemberDayCateSales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT  A.member_seqno";
        $query .= "\n          ,A.input_date";
        $query .= "\n          ,A.cate_top";
        $query .= "\n          ,A.count";
        $query .= "\n          ,A.pay_price";
        $query .= "\n     FROM  day_cate_sales_stats AS A";
        $query .= "\n    WHERE  A.member_seqno = %s";
        $query .= "\n      AND  A.cate_top     = %s";
        $query .= "\n ORDER BY  A.input_date DESC";
        $query .= "\n    LIMIT  1";

        $query = sprintf($query, $param["member_seqno"]
                               , $param["cate_top"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 카테고리별 집계 정보 입력
     *
     * @param $conn = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 쿼리 실행결과
     */
    function insertMemberDayCateSales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO day_cate_sales_stats (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,input_date";
        $query .= "\n     ,cate_top";
        $query .= "\n     ,count";
        $query .= "\n     ,pay_price";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_date"]
                                , $param["cate_top"]
                                , $param["count"]
                                , $param["pay_price"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 카테고리별 집계 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 입력값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMemberDayCateSales($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  day_cate_sales_stats as A";
        $query .= "\n    SET  A.member_seqno = A.member_seqno";
        if ($this->blankParameterCheck($param, "count")) {
            $query .= "\n        ,A.count  = ";
            $query .= $param["count"];
        }
        if ($this->blankParameterCheck($param, "pay_price")) {
            $query .= "\n        ,A.pay_price  = ";
            $query .= $param["pay_price"];
        }

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.input_date   = %s";
        $query .= "\n    AND  A.cate_top     = %s";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_date"]
                                , $param["cate_top"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 테이블 값 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 입력값 파라미터
     *
     * @return 쿼리실행결과
     */
    function updateMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  member as A";
        $query .= "\n    SET  A.member_seqno = A.member_seqno";
        $query .= "\n        ,final_modi_date = now()";
        if ($this->blankParameterCheck($param, "final_order_date")) {
            $query .= "\n        ,A.final_order_date  = ";
            $query .= $param["final_order_date"];
        }
        /*
        if ($this->blankParameterCheck($param, "first_order_date")) {
            $query .= "\n        ,A.first_order_date = ";
            $query .= $param["first_order_date"];
        }
        */
        if ($this->blankParameterCheck($param, "cumul_sales_price")) {
            $query .= "\n        ,A.cumul_sales_price = IFNULL(A.cumul_sales_price, 0) + ";
            $query .= $param["cumul_sales_price"];
        }
        if ($this->blankParameterCheck($param, "order_lack_price")) {
            $query .= "\n        ,A.order_lack_price = ";
            $query .= $param["order_lack_price"];
        }
        if ($this->blankParameterCheck($param, "prepay_price_card")) {
            $query .= "\n        ,A.prepay_price_card = IFNULL(A.prepay_price_card, 0) - ";
            $query .= $param["prepay_price_card"];
        }
        if ($this->blankParameterCheck($param, "prepay_price_money")) {
            $query .= "\n        ,A.prepay_price_money = IFNULL(A.prepay_price_money, 0) - ";
            $query .= $param["prepay_price_money"];
        }
        /*
        if ($this->blankParameterCheck($param, "own_point")) {
            $query .= "\n        ,A.own_point = ";
            $query .= $param["own_point"];
        }
        */
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 사용자 장바구니 갯수
     *
     * @param $conn = connection identifier
     * @param $member_seqno = 사용자 일련번호
     *
     * @return 검색결과
     */
    function selectCartCount($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  COUNT(*) AS cnt";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  member_seqno = %s";
        $query .= "\n    AND  order_state = '110'";

        $query  = sprintf($query, $member_seqno);

        return $conn->Execute($query);
    }
 
    /**
     * @brief 선입금 충전
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function updateMemberPrepay($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  prepay_price_money = IFNULL(prepay_price_money, 0) + %s";
        $query .= "\n           ,prepay_price_card = IFNULL(prepay_price_card, 0) + %s";
        $query .= "\n           ,final_modi_date = now()";
        if ($this->blankParameterCheck($param, "cumul_sales_price")) {
            $query .= "\n    ,cumul_sales_price = cumul_sales_price - ";
            $query .= $param["cumul_sales_price"];
        }
        $query .= "\n     WHERE  member_seqno = %s";

        $query = sprintf($query, $param["prepay_price_money"] ?? 0,
                                 $param["prepay_price_card"] ?? 0,
                                 $param["member_seqno"]);

         $resultSet = $conn->Execute($query);
 
        if (!$resultSet) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief sms 보내기
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function sendSms($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nINSERT INTO SC_TRAN (";
        $query .= "\n  TR_SENDDATE";
        $query .= "\n, TR_SENDSTAT";
        $query .= "\n, TR_MSGTYPE";
        $query .= "\n, TR_PHONE";
        $query .= "\n, TR_CALLBACK";
        $query .= "\n, TR_MSG) ";
        $query .= "\nVALUES (";
        $query .= "\n  NOW()";
        $query .= "\n, '0'";
        $query .= "\n, '0'";
        $query .= "\n, %s";
        $query .= "\n, %s";
        $query .= "\n, %s)";

        $query = sprintf($query,
                         $param["tr_phone"],
                         $param["tr_callback"],
                         $param["tr_msg"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 카테고리명 검색
     *
     * @param $conn = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return 카테고리명
     */
    function selectCateName($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $temp = array();
        $temp["col"] = "cate_name";
        $temp["table"] = "cate";
        $temp["where"]["sortcode"] = $cate_sortcode;
        $rs = $this->selectData($conn, $temp);
        return $rs->fields["cate_name"];
    }

    /**
     * @brief 주문상태값 검색
     *
     * @param $conn = connection identifier
     *
     * @return 카테고리명
     */
    function selectStateAdmin($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n   SELECT  A.state_code";
        $query .= "\n          ,A.front_state_name";
        $query .= "\n     FROM  state_admin AS A";
        $query .= "\n ORDER BY  A.state_code";

        $rs = $conn->Execute($query);

        return $rs;
    }
    /**
     * @brief 주문상태값 검색
     *
     * @param $conn = connection identifier
     *
     * @return 카테고리명
     */
    function selectStateAdminName($conn,$state) {
        /*if ($this->connectionCheck($conn) === false) {
            return false; 
        }*/

        $query  = "\n   SELECT  front_state_name";
        $query .= "\n     FROM  state_admin";
        $query .= "\n  where state_code = '%s'";

        $query  = sprintf($query, $state);

        $rs = $conn->Execute($query);

        return $rs->fields; ;
    }

    /**
     * @brief 주문상태 구분값 검색
     *
     * @param $conn = connection identifier
     *
     * @return 카테고리명
     */
    function selectStateAdminDvs($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $query  = "\n   SELECT  DISTINCT A.front_dvs AS dvs";
        $query .= "\n     FROM  state_admin AS A";
        $query .= "\n ORDER BY  A.state_code";

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @breif 상태_관리 테이블에서 해당 상태에 해당하는
     * 범위 최소/최대값 검색
     *
     * @param $conn = db connection
     * @param $dvs  = 상태구분
     *
     * @return 최소/최대값
     */
    function selectStateAdminRange($conn, $dvs) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $dvs = $this->parameterEscape($conn, $dvs);

        $query  = "\n SELECT  MIN(A.state_code + 0) AS min";
        $query .= "\n        ,MAX(A.state_code + 0) AS max";
        $query .= "\n   FROM  state_admin AS A";
        $query .= "\n  WHERE  A.front_dvs IN (%s)";
        $query .= "\n    AND  A.front_state_name NOT IN ('주문취소', '주문대기');";

        $query  = sprintf($query, $dvs);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 수량 회원별 할인 가격 검색
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAmtMemberCateSale($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* ProductCommonDAO.php, 회원특별할인 검색 */";
        $query .= "\n         rate";
        $query .= "\n        ,aplc_price";
        $query .= "\n   FROM  amt_member_cate_sale";
        $query .= "\n  WHERE  1 = 1";

        $query .= "\n    AND member_seqno = %s";
        $query .= "\n    AND cate_sortcode = %s";
        $query .= "\n    AND cate_paper_mpcode = %s";
        $query .= "\n    AND cate_stan_mpcode = %s";
        $query .= "\n    AND cate_beforeside_print_mpcode = %s";
        $query .= "\n    AND cate_beforeside_add_print_mpcode = %s";
        $query .= "\n    AND cate_aftside_print_mpcode = %s";
        $query .= "\n    AND cate_aftside_add_print_mpcode = %s";
        $query .= "\n    AND amt = %s";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["cate_sortcode"]
                                , $param["paper_mpcode"]
                                , $param["stan_mpcode"]
                                , $param["bef_print_mpcode"]
                                , $param["bef_add_print_mpcode"]
                                , $param["aft_print_mpcode"]
                                , $param["aft_add_print_mpcode"]
                                , $param["amt"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    ///////////////////////////////////////////////////////////////////////////
    //--------------- 이하 추가

    /**
     * @brief 소셜회원 아이디 검사
     * @detail 멤버이름, 메일, 가입경로를 통해 검사한다.
     *
     */
    function selectSocialAcc($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nSELECT  A.member_seqno";
        $query .= "\n  FROM  member AS A";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param, "name")) {
            $query .= "\n    AND  A.member_name = ";
            $query .= $param["name"];
        }
        // 180102 member테이블에 id컬럼추가 : 기존mail 에서 변경
        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.id = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            //$query .= "\n    AND  A.join_path = ";
            //$query .= $param["join_path"];
        }

        $rs = $conn->Execute($query);

        return $rs->fields["member_seqno"];
    }

    /**
     * @brief 소셜회원 부계정 검사
     * @detail 메일, 가입경로를 통해 검사한다.
     *
     */
    function selectSocialAccSub($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nSELECT  A.member_seqno";
        $query .= "\n  FROM  gprinting.member_sub_id AS A";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.mail = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            $query .= "\n    AND  A.join_path = ";
            $query .= $param["join_path"];
        }

        $rs = $conn->Execute($query);

        return $rs->fields["member_seqno"];
    }

    /**
     * @brief 소셜로그인 회원정보 등록
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertSocialAcc($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO gprinting.member (";
        $query .= "\n        member_name";
        $query .= "\n       ,withdraw_yn";
        $query .= "\n       ,id";
        $query .= "\n       ,mail";
        $query .= "\n       ,first_join_date";
        $query .= "\n       ,grade";
        $query .= "\n       ,member_dvs";
        $query .= "\n       ,join_path";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,'개인회원'";
        $query .= "\n       ,%s)";
        $query  = sprintf($query, $param["name"]
                                , $param["withdraw_yn"] 
                                , $param["mail"] 
                                , $param["mail"] 
                                , $param["grade"] 
                                , $param["join_path"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 통상로그인 회원정보 등록
     *
     * @param $conn  = connection identifier
     * @param $param = 파라미터
     *
     * @return 검색결과
     */
    function insertComMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO gprinting.member (";
        $query .= "\n        mail";
        $query .= "\n       ,passwd";
        $query .= "\n       ,grade";
        $query .= "\n       ,first_join_date";
        $query .= "\n       ,withdraw_yn";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,%s)";
        $query  = sprintf($query, $param["mail"]
                                , $param["passwd"] 
                                , $param["grade"] 
                                , $param["withdraw_yn"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 소셜회원 기등록 검사(연동용)
     *
     */
    function selectDuplSocialAcc($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nSELECT  COUNT(*) AS cnt";
        $query .= "\n  FROM  gprinting.member AS A";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.mail = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            $query .= "\n    AND  A.join_path = ";
            $query .= $param["join_path"];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 소셜회원 기등록 서브테이블 검사(연동용)
     *
     */
    function selectDuplSocialSub($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nSELECT  COUNT(*) AS sub_cnt";
        $query .= "\n  FROM  gprinting.member_sub_id AS A";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.mail = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            $query .= "\n    AND  A.join_path = ";
            $query .= $param["join_path"];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 소셜회원 연동정보 등록
     *
     * @param $conn  = connection identifier
     * @param $param = 파라미터
     *
     * @return 검색결과
     */
    function insertSocialAccLink($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO gprinting.member_sub_id (";
        $query .= "\n        mail";
        $query .= "\n       ,join_path";
        $query .= "\n       ,member_seqno";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";
        $query  = sprintf($query, $param["mail"]
                                , $param["join_path"]
                                , $param["member_seqno"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 회원 아이디 중복 검사
     *
     */
    function selectDuplEmail($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\nSELECT  A.member_seqno";
        $query .= "\n  FROM  gprinting.member AS A";
        $query .= "\n WHERE  1 = 1 ";

        if ($this->blankParameterCheck($param, "mail")) {
            $query .= "\n    AND  A.mail = ";
            $query .= $param["mail"];
        }
        if ($this->blankParameterCheck($param, "join_path")) {
            $query .= "\n    AND  A.join_path = ";
            $query .= $param["join_path"];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고리 정보 검색
     *
     * @param $conn          = connection identifer
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function selectCateInfo($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  cate_name";
        $query .= "\n        ,flattyp_yn";
        $query .= "\n        ,tmpt_dvs";
        $query .= "\n        ,tot_name";
        $query .= "\n        ,mono_dvs";
        $query .= "\n        ,typset_way";
        $query .= "\n        ,amt_unit";
        $query .= "\n   FROM  cate";
        $query .= "\n  WHERE  sortcode = %s";

        $query  = sprintf($query, $conn->param($cate_sortcode));
        $rs = $conn->Execute($query, [$cate_sortcode]);

        return $rs->fields;
    }

    /**
     * @brief 회원 선입금 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPrepay($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n    SELECT  /* FrontCommonDAO.inc - 선입금 검색 query */";
        $query .= "\n            prepay_price_money";
        $query .= "\n           ,prepay_price_card";
        $query .= "\n      FROM  member";
        $query .= "\n     WHERE  member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $rs = $conn->Execute($query, [$param["member_seqno"]]);

        return $rs->fields;
    }

    function selectChannelInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n    SELECT  * ";
        $query .= "\n      FROM  cpn_admin";
        $query .= "\n     WHERE  sell_site = '%s'";

        $query  = sprintf($query, $param["sell_site"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    function selectMemberCalculInfo($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT  B.bank_name ";
        $query .= "\n          ,B.ba_num ";
        $query .= "\n     FROM  member  AS A ";
        $query .= "\nLEFT JOIN  virt_ba_admin AS B ";
        $query .= "\n       ON  A.member_seqno = B.member_seqno AND use_yn = 'Y' ";
        $query .= "\n    WHERE  1 = 1 ";

        //회원일련번호
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n      AND  A.member_seqno = $param[member_seqno] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 선입금액 검색, 다른 작업과 겹칠수도 있어서 row lock 걸음
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPrepayLock($conn, $member_seqno) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  /* MemberDlvrDAO.inc - 회원 선입금액 검색 + row lock */";
        $query .= "\n         A.prepay_price_money";
        $query .= "\n        ,A.prepay_price_card";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  member_seqno = %s";
        $query .= "\n FOR UPDATE";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 가격 테이블에서 규격 맵핑코드 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateStanMpcodeByPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $table_name = $param["table_name"];
        if (empty($param["table_name"])) {
            $table_name = "ply_price";
        }

        $query  = "\n SELECT  DISTINCT A.cate_stan_mpcode";
        $query .= "\n   FROM  %s AS A";
        $query .= "\n  WHERE  A.cate_sortcode = %s ";

        if($conn->param($param["paper_mpcode"] != null)) {
            $query .= "\n    AND  A.cate_paper_mpcode = " . $conn->param($param["paper_mpcode"]);
        }
        $query  = sprintf($query, $table_name
                                , $conn->param($param["cate_sortcode"]));
        //$query .= "\n  ORDER BY A.seq ";

        $temp = [
             $param["cate_sortcode"]
            ,$param["paper_mpcode"]
        ];
        //$conn->debug =1 ;
        $rs = $conn->Execute($query, $temp);
        //$conn->debug =0;
        return $rs;
    }

    /**
     * @brief 회원 가상계좌정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
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

        $query  = "\nSELECT  /* 회원 가상계좌정보 query */ ";
        $query .= "\n        ba_num ";
        $query .= "\n       ,use_yn ";
        $query .= "\n       ,bank_name ";
        $query .= "\n       ,depo_name ";
        $query .= "\n  FROM  virt_ba_admin"; // TODO 추후 이부분 수정필요
        $query .= "\n WHERE  member_seqno = " . $param["member_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 선입금, 부족금액 정보 가져오기
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    /*
    function selectPrepayPrice($conn, $id) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $id = $this->parameterEscape($conn, $id);

        $query  = "\n SELECT get_prepay_channel(%s, %s) ";
        $query = sprintf($query,
            $id, $_SERVER["SELL_SITE"]);

        return $conn->Execute($query);
    }
    */

    function selectPrepayPrice($conn, $id) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $id = $this->parameterEscape($conn, $id);

        $date = date("Y-m-d");

        $query  = "\n SELECT get_prepay(%s ,'" . $date . "') ";
        /*
        $query  = "\n SELECT prepay_price_money ";
        $query  .= "\n FROM member ";
        $query  .= "\n WHERE id = %s ";
        */
        $query = sprintf($query,
            $id);


        $id = $_SESSION["id"];
        if($id=='sungbang_test'){
            // 개발 테스트용
            $rskaka = new stdClass();
            $rskaka->fields = [
                0 => 99999999,
                "get_prepay('sungbang_test' ,'".date("Y-m-d")."')"=>99999999
            ];
            return $rskaka;
        }
        $result = $conn->Execute($query);
        return $result;
    }

    /**
     * @brief 택배비 조회시 도서지역인지 확인
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectIslandParcelCost($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT DISTINCT price ";
        $query .= "\n FROM  CJparcel_islands ";
        $query .= "\n WHERE  new_zipcode = %s";

        $query  = sprintf($query, $conn->param($param['zipcode']));

        return $conn->Execute($query, [$param["zipcode"]]);
    }

    /**
     * @brief 택배비 조회시 도서지역인지 확인
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectOrderCatePage($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT B.page, A.cate_sortcode ";
        $query .= "\n FROM  order_common AS A";
        $query .= "\n INNER JOIN  cate_info AS B ON A.cate_sortcode = B.cate_sortcode ";
        $query .= "\n WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param['order_common_seqno']);

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 대분류 별명 검색
     *
     * @param $conn          = connection identifer
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function selectCateNick($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT nick";
        $query .= "\n   FROM cate";
        $query .= "\n  WHERE sortcode   = %s";
        $query .= "\n    AND cate_level = '1'";

        $query  = sprintf($query, $cate_sortcode);

        $rs = $conn->Execute($query);

        return $rs->fields["nick"];
    }

    /**
     * @brief 시퀀스 증가용 마지막 주문번호 검색
     *
     * @param $conn  = connection identifer
     *
     * @return 마지막 숫자
     */
    function selectOrderCommonLastNum($conn) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        date_default_timezone_set("Asia/Seoul");
        $today = date("Y-m-d");

        $query  = "\n   SELECT ";
        $query .= "\n          order_num";
        $query .= "\n     FROM order_common";
        $query .= "\n    WHERE '%s 00:00:00' <= order_regi_date";
        $query .= "\n      AND order_regi_date <= '%s 23:59:59'";
        $query .= "\n ORDER BY order_common_seqno DESC";
        $query .= "\n    LIMIT 1";

        $query  = sprintf($query, $today, $today);

        $rs = $conn->Execute($query);
        if ($rs->EOF) {
            $last_num = 1;
        } else {
            $last_num = intval(substr($rs->fields["order_num"], 11)) + 1;
        }

        return $last_num;
    }

    function selectPrintTmptInfo($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  A.side_dvs";
        $query .= "\n        ,A.beforeside_tmpt";
        $query .= "\n        ,A.aftside_tmpt";
        $query .= "\n        ,A.add_tmpt";
        $query .= "\n        ,A.tot_tmpt";
        $query .= "\n   FROM  prdt_print AS A";
        $query .= "\n        ,cate_print AS B";
        $query .= "\n  WHERE  A.prdt_print_seqno = B.prdt_print_seqno";
        $query .= "\n    AND  B.mpcode IN (%s)";

        $query  = sprintf($query, $mpcode);

        $rs = $conn->Execute($query);

        return $rs;
    }

	// 출석체크 세팅 DB 검색
	function selectAttendanceSetting($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  *";
        $query .= "\n   FROM  attendance_setting";
        $query .= "\n  WHERE  1 ";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }


	// 어제 출석체크 check
    function ydAttendanceCheck($conn, $param) {

		$todayDate = date('Y-m-d'); // 오늘 날짜
		$yday = date('Y-m-d', strtotime($todayDate . ' -1 day'));

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT * ";
		$query  .= "\n FROM attendance ";
		$query  .= "\n WHERE ae_seqno = %s and substring(ae_datetime,1,10) = '%s' ";
		
		$query  = sprintf($query, $param["ae_seqno"], $yday);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }


	// 오늘 출석체크 check
    function todayAttendanceCheck($conn, $param) {
		$todayDate = date('Y-m-d'); // 오늘날짜
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT * ";
		$query  .= "\n FROM attendance ";
		$query  .= "\n WHERE ae_seqno = %s and substring(ae_datetime,1,10) = %s ";
		$query  = sprintf($query, $param["ae_seqno"], "'{$todayDate}'");

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

	// 오늘 출석체크 check2
    function todayAttendanceCheck2($conn, $param, $lastDayx) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT * ";
	    $query  .= "\n FROM attendance ";
		$query  .= "\n WHERE ae_seqno = %s and substring(ae_datetime,1,10) = %s ";
		$query  = sprintf($query, $param["ae_seqno"], "'{$lastDayx}'");

        $rs = $conn->Execute($query);

        return $rs->fields;
    }
	
	// 포인트 내역 합계
	function get_point_sum($conn, $member_seqno) {

			if ($this->connectionCheck($conn) === false) {
				return false;
			}
			$member_seqno = $this->parameterEscape($conn, $member_seqno);

			$query  = "\n SELECT po_mb_point as sum_point ";
			$query .= "\n from member_point ";
			$query .= "\n where mb_seqno = %s ";
			$query .= "\n order by po_datetime desc";
			$query .= "\n limit 1";
			$query  = sprintf($query, $member_seqno);

			$rs = $conn->Execute($query);
            //echo $query;
			return $rs->fields;
	}

    // 포인트 내역 합계
	function get_point_list($conn, $member_seqno) {

			if ($this->connectionCheck($conn) === false) {
				return false;
			}
			$member_seqno = $this->parameterEscape($conn, $member_seqno);

			$query  = "\n SELECT * ";
			$query .= "\n from member_point ";
			$query .= "\n where mb_seqno = %s ";
			$query .= "\n order by po_datetime desc";
			$query  = sprintf($query, $member_seqno);

            //echo $query;
			$rs = $conn->Execute($query);

			return $rs;
	}

	// 오늘 출석체크 insert
	function todayAttendanceInsert($conn, $param) {
		$todayDate = date('Y-m-d'); // 오늘날짜
		$todayDate2 = date('Y-m-d H:i:s'); // 오늘날짜 시간
		if ($this->connectionCheck($conn) === false) {
			return false;
		}

		$sql_point = $param["point"];
		
		// 어제 출석했다면
		if($param["ydCheck"] == 1) {
			// 지난 개근체크에 오늘 합산
			$sql_reset = $param["ae_reset1"] + 1;
			$sql_reset2 = $param["ae_reset2"] + 1;
			
			if ($sql_reset == 10) { // 10일 개근
				$sql_reset  = "0"; 
				$sql_point  = $sql_point + $param["reset1"];
			}
			
			if ($sql_reset2 == 30) { // 30일 개근
				$sql_reset2 = "0"; 
				$sql_point  = $sql_point + $param["reset2"];
			}
		} else { // 출석하지 않았다면
			// 리셋
			$sql_reset  = "1";
			$sql_reset2 = "1";
		}

		
		$query1  = "\n INSERT INTO attendance ";
		$query1 .= "\n (ae_seqno, ae_point, ae_datetime, ae_reset1, ae_reset2) VALUES (%s, %s, %s, %s, %s) ";
		$query1  = sprintf($query1, $param["ae_seqno"], "'$sql_point'", "'{$todayDate}'", "'$sql_reset'", "'$sql_reset2'");
		
		$result1 = $conn->Execute($query1);
		
		$su = preg_replace("/[^0-9]*/s", "", $param['point_sum']);
		$sum_point = $sql_point + $su;
		
		$query2  = "\n INSERT INTO member_point ";
		$query2 .= "\n (mb_seqno, mb_email, po_datetime, po_content, po_point, po_use_point, po_mb_point, po_rel_table, po_rel_action) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s) ";
		$query2  = sprintf($query2, $param["ae_seqno"], "'{$_POST['mb_id']}'", "'{$todayDate2}'", "'출석체크 포인트 지급'", $sql_point, 0, $sum_point, "'@attendance'", "'@출석체크'");
		
		$result2 = $conn->Execute($query2);

	}
	// 출석체크 설정 update function
    function updateAttendance($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $query  = "\n    UPDATE  attendance_setting ";
        $query .= "\n       SET  ae_point = %s, ae_sequence = %s, ae_sequence2 = %s";
        $query = sprintf($query, $param["one"], $param["two"], $param["three"]);
        $resultSet = $conn->Execute($query);
    }
	
	// 포인트 설정 update function
    function updatePoint($conn, $param, $rs, $dao) {
		$todayDate2 = date('Y-m-d H:i:s'); // 오늘날짜 시간
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
		

		$point = $dao->get_point_sum($conn, $rs["member_seqno"]);
		// 현재 전체 포인트 내역 가져오기
		$point_sum = $point['sum_point'];
		

		$su = preg_replace("/[^0-9]*/s", "", $point_sum);
		$sum_point = $su;
		
		if($param['add_minus_check'] === 'add'){
			$amc = '+';
		} else {
			$amc = '-';
		}



        //인젝션 어택 방지
       $query  = "\n INSERT INTO member_point (";
        $query .= "\n      mb_seqno";
        $query .= "\n     ,mb_email";
        $query .= "\n     ,po_datetime";
        $query .= "\n     ,po_content";
        $query .= "\n     ,po_point";
        $query .= "\n     ,po_use_point";
        $query .= "\n     ,po_mb_point";
        $query .= "\n     ,po_rel_table";
        $query .= "\n     ,po_rel_action";
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
        $query .= "\n )";
		
		$query = sprintf($query, $rs["member_seqno"], "'{$rs['id']}'", "'{$todayDate2}'", "'{$param['add_minus_reason']}'", $amc.$param['send_points'], 0, $sum_point.$amc.$param['send_points'], "'@admin'", "'{$param['add_minus_reason']}'");
		
		
        $resultSet = $conn->Execute($query);

       // return $query;
    }
    // 포인트 설정 update function
    function updateManagePoint($conn, $param, $dao) {
		$todayDate2 = date('Y-m-d H:i:s'); // 오늘날짜 시간
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
		

		$point = $dao->get_point_sum($conn, $param["member_seqno"]);
		// 현재 전체 포인트 내역 가져오기
		$point_sum = $point['sum_point'];
		

		$su = preg_replace("/[^0-9]*/s", "", $point_sum);
		$sum_point = $su;
		
		if($param['add_minus_check'] === 'add'){
			$amc = '+';
		} else {
			$amc = '-';
		}



        //인젝션 어택 방지
       $query  = "\n INSERT INTO member_point (";
        $query .= "\n      mb_seqno";
        $query .= "\n     ,mb_email";
        $query .= "\n     ,po_datetime";
        $query .= "\n     ,po_content";
        $query .= "\n     ,po_point";
        $query .= "\n     ,po_use_point";
        $query .= "\n     ,po_mb_point";
        $query .= "\n     ,po_rel_table";
        $query .= "\n     ,po_rel_action";
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
        $query .= "\n )";
		
		$query = sprintf($query, $param["member_seqno"], "''", "'{$todayDate2}'", "'{$param['add_minus_reason']}'", $amc.$param['send_points'], 0, $sum_point.$amc.$param['send_points'], "'@admin'", "'{$param['add_minus_reason']}'");
		
		
        $resultSet = $conn->Execute($query);

        //return $query;
    }

    // 포인트 관리 리스트 update function
    
    function updateMemberChk($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
  
        $query  = "\n    UPDATE  member ";
        $query .= "\n       SET  chk_yn = 'Y' ";
        $query .= "\n     WHERE  member_seqno = %s ";

        $query = sprintf($query, $param["member_seqno"]);

        $resultSet = $conn->Execute($query);
 
        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }


    // 포인트 설정 update function
    function updatePoint2($conn, $param, $rs, $dao) {
		$todayDate2 = date('Y-m-d H:i:s'); // 오늘날짜 시간
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
		

		$point = $dao->get_point_sum($conn, $rs["member_seqno"]);
		// 현재 전체 포인트 내역 가져오기
		$point_sum = $point['sum_point'];
		

		$su = preg_replace("/[^0-9]*/s", "", $point_sum);
		$sum_point = $su;
		
		if($param['add_minus_check'] === 'add'){
			$amc = '+';
		} else {
			$amc = '-';
		}



        //인젝션 어택 방지
       $query  = "\n INSERT INTO member_point (";
        $query .= "\n      mb_seqno";
        $query .= "\n     ,mb_email";
        $query .= "\n     ,po_datetime";
        $query .= "\n     ,po_content";
        $query .= "\n     ,po_point";
        $query .= "\n     ,po_use_point";
        $query .= "\n     ,po_mb_point";
        $query .= "\n     ,po_rel_table";
        $query .= "\n     ,po_rel_jumunNo";
        $query .= "\n     ,po_rel_action";
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
        $query .= "\n )";
		
		$query = sprintf($query, $rs["member_seqno"], "'{$rs['id']}'", "'{$todayDate2}'", "'{$param['add_minus_reason']}'", $amc.$param['send_points'], 0, $sum_point.$amc.$param['send_points'], "'{$rs['id']}'", "'{$param['order_num']}'", "'{$param['add_minus_reason']}'");
		
		
        $resultSet = $conn->Execute($query);
    }
	// 포인트 설정 member search - 이메일로 찾는다.
    function selectMemberInfoPoint($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  A.mail";
        $query .= "\n        ,A.is_except";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.member_seqno";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,B.tel_num";
        $query .= "\n        ,B.cell_num";
        $query .= "\n        ,B.zipcode";
        $query .= "\n        ,B.addr";
        $query .= "\n        ,B.addr_detail";
        $query .= "\n   FROM  member AS A";
        $query .= "\n   LEFT JOIN member_dlvr AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  WHERE  1 = 1 ";
        $query .= "\n    AND  A.id = '%s' ";
        $query .= "\n   ORDER BY B.basic_yn DESC LIMIT 1 ";

        $query  = sprintf($query, $param['mb_id_point']);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    // 포인트 설정 member search - 멤버seq로 찾는다.
    function selectMemberInfoPoint2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  A.mail";
        $query .= "\n        ,A.is_except";
        $query .= "\n        ,A.id";
        $query .= "\n        ,A.member_seqno";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,B.tel_num";
        $query .= "\n        ,B.cell_num";
        $query .= "\n        ,B.zipcode";
        $query .= "\n        ,B.addr";
        $query .= "\n        ,B.addr_detail";
        $query .= "\n   FROM  member AS A";
        $query .= "\n   LEFT JOIN member_dlvr AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  WHERE  1 = 1 ";
        $query .= "\n    AND  A.member_seqno = '%s' ";
        $query .= "\n   ORDER BY B.basic_yn DESC LIMIT 1 ";

        $query  = sprintf($query, $param['member_seqno']);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    function checkProduct($conn, $order_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $val1 = 'ST';
        $val2 = 'NC';

        $query  = "\n SELECT  count(*) AS num";     
        $query .= "\n FROM  order_common AS A";
        $query .= "\n  WHERE  1 = 1 ";
        $query .= "\n  AND  order_common_seqno  = '".$order_seqno."' ";
        $query .= "\n  AND  (order_detail LIKE '%수입명함%'";
        $query .= "\n  OR   order_detail LIKE '%일반명함%')";

       // echo $query;

        return $conn->Execute($query);

        //return $rs->fields;
    }

	function selectMemberInfo2($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        if($param["keyword"] == "") $search_dvs = "";
        $seqno = $param['member_seqno'] ;

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  ="\nSELECT  * ";
        }

        $query .="\n  FROM  attendance  ";
        $query .="\n  WHERE 1 = 1 ";
       
        if($seqno  != ""){
            $from = "'".str_replace("'","", $param['date_from'])."'";
            $to = "'".str_replace("'","", $param['date_to'])."'";
            $query .="\n  and ae_datetime between $from and $to "; 
        }
        if($seqno != ""){
           $seqno = $param['member_seqno'];
           $query .="\n  and ae_seqno = ".$seqno." "; 
        }
       
        $query .="\n  order by ae_datetime desc";

        

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

       /* if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        } */

       //echo $query;
        return $conn->Execute($query);
    }
	
	
	function selectMemberInfo3($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        $seqno = $param['member_seqno'];
        $param_from = $param['date_from'];
        $param_to = $param['date_to'];

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
            $query  ="\nSELECT  * ";
        }

        $query .="\n  FROM  member_point  ";
        $query .="\n  WHERE 1 = 1 ";

        if($seqno != ""){
           $query .="\n  and mb_seqno = ".$seqno." "; 
        }
        
        if($param_from != "" && $param_to != "" && $seqno != "" ) {
            $from = "'".str_replace("'","", $param['date_from'])." 00:00:00'";
            $to = "'".str_replace("'","", $param['date_to'])." 23:59:59'";
            $query .="\n  and po_datetime between $from and $to "; 

        }
       


        $query .="\n  order by po_datetime desc";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ" ) { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

       // $query  = sprintf($query, $param["member_seqno"]);

        // echo $query;
        return $conn->Execute($query);
    }

    function selectsumpoint($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        $seqno = $param['member_seqno'];
        $param_from = $param['date_from'];
        $param_to = $param['date_to'];

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
            $query  ="\nselect case when sign(po_point) = '1' then po_point else 0 end as result1, case when sign(po_point) = '-1' then abs(po_point) else 0 end as result2 ";
        }

        $query .="\n  FROM  member_point  ";
        $query .="\n  WHERE 1 = 1 ";

        if($seqno != ""){
           $query .="\n  and mb_seqno = ".$seqno." "; 
        }
        
        if($param_from != "" && $param_to != "" && $seqno != "") {
            $from = "'".str_replace("'","", $param['date_from'])." 00:00:00'";
            $to = "'".str_replace("'","", $param['date_to'])." 23:59:59'";
            $query .="\n  and po_datetime between $from and $to "; 

        }
       


        $query .="\n  order by po_datetime desc";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        /*if ($dvs == "SEQ" ) { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        } */

        $query  = sprintf($query, $param["member_seqno"]);

       // echo $query;
       return $conn->Execute($query);
    }

    function selectPointUseList($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        $seqno = $param['member_seqno'];
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
            $query  ="\nSELECT  * ";
        }

        $query .="\n  FROM  order_common ";
        $query .="\n  WHERE use_point_price != 0 ";

        if($seqno != ""){
           //$seqno = $param['member_seqno'];
           $query .="\n  and member_seqno = '".$seqno."'"; 
        }

        if($seqno != ""){
            $from = "'".str_replace("'","", $param['date_from'])." 00:00:00'";
            $to = "'".str_replace("'","", $param['date_to'])." 23:59:59'";
            $query .="\n  and order_regi_date between $from and $to "; 
        }

        $query .="\n  order by order_common_seqno desc";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
/*
        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
*/
       // echo $query;
        return $conn->Execute($query);
    }
	
     function selectPointUseList2($conn, $dvs, $param) {

        $search_dvs  = $param["search_dvs"];
        $seqno = $param['member_seqno'];
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
            $query  ="\nSELECT  * ";
        }

        $query .="\n  FROM  order_common ";
        $query .="\n  WHERE use_point_price != 0 ";

        if($seqno != ""){
           //$seqno = $param['member_seqno'];
           $query .="\n  and member_seqno = '".$seqno."'"; 
        }

        if($seqno != ""){
            $from = "'".str_replace("'","", $param['date_from'])." 00:00:00'";
            $to = "'".str_replace("'","", $param['date_to'])." 23:59:59'";
            $query .="\n  and order_regi_date between $from and $to "; 
        }

        $query .="\n  order by order_common_seqno desc";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ" && $seqno != "") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        //echo $query;
        return $conn->Execute($query);
    }


	


}



function makePoint($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
	$html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
	
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

	$rs2 = $dao->selectMemberInfo($conn, $rs->fields["mb_seqno"]);
	$point = $dao->get_point_sum($conn, $rs->fields["mb_seqno"]);
	// 현재 전체 포인트 내역 가져오기
	$point_sum = $point['sum_point'];
    if(number_format($rs->fields["po_point"])> 0 ){
        $add_point = number_format($rs->fields["po_point"]);
        $use_point = 0;
    }else{
        $add_point = 0;
        $use_point = number_format($rs->fields["po_point"]);
    }

        $rs_html .= sprintf($html, $class, 
                $i,
            $rs2['id'],
            $rs2['member_name'],
            $rs->fields["po_content"],
            $rs->fields["po_datetime"],
            $add_point,
            $use_point, 
			number_format($rs->fields['po_mb_point'])
			);
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeUsePoint($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
	$html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
	
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

	$rs2 = $dao->selectMemberInfo($conn, $rs->fields["member_seqno"]);
	$point = $dao->get_point_sum($conn, $rs->fields["member_seqno"]);
	// 현재 전체 포인트 내역 가져오기
	$point_sum = $point['sum_point'];
    $stateName = $dao->selectStateAdminName($conn, $rs->fields["order_state"]);

        $rs_html .= sprintf($html, $class, 
            $rs->fields["order_num"],
            $rs->fields['order_regi_date'],
            $rs2['member_name'],
            $rs->fields["title"],
            $rs->fields["order_detail"],
            $stateName["front_state_name"],
            $rs->fields["count"],
            $rs->fields["amt"],
            $rs->fields["pay_price"]+ $rs->fields["use_point_price"],
            $rs->fields["pay_price"],
            $rs->fields["use_point_price"],
			);
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}


function usePointCheck($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	
    $useOne = 0;
    $usePoint = 0;
	
    while ($rs && !$rs->EOF) {

        $useOne += $rs->fields["pay_price"];
        $usePoint += $rs->fields["use_point_price"];
        $rs->moveNext();
    }

    return $useOne . "♪" . $usePoint;
}

function selectPointCheck($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	
    $addpoint = 0;
    $usePoint = 0;
	
    while ($rs && !$rs->EOF) {

        $addpoint += $rs->fields["result1"];
        $usePoint += $rs->fields["result2"];
        $rs->moveNext();
    }

    return $addpoint . "♪" . $usePoint;
}

function makeAttendance($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
	
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

	$rs2 = $dao->selectMemberInfo($conn, $rs->fields["ae_seqno"]);
	$point = $dao->get_point_sum($conn, $rs->fields["ae_seqno"]);
	// 현재 전체 포인트 내역 가져오기
	$point_sum = $point['sum_point'];

        $rs_html .= sprintf($html, $class, 
                $i,
            $rs2['id'],
            $rs2['member_name'],
            $rs->fields["ae_datetime"],
            $rs->fields["ae_reset2"],
            number_format($rs->fields["ae_point"]));
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeMemberHtml($conn, $dao, $rs, $param) {

    $nimdao = new NimdaCommonDAO();
	
    if (!$rs) {
        return false;
    }
	

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td><input type='button' onclick='alertMSG(%s)' value='작성' />&nbsp;<input type='button' onclick='alertLIST(%s)' value='내역보기' /></td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
	
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
    $param['member_seqno'] = $rs->fields['member_seqno'];
    $count = $dao->selectMemberConInfo($conn, "COUNT", $rs->fields['member_seqno']);

    
    $info = $dao->selectMemberConInfo($conn, "SEQ", $rs->fields['member_seqno']);
    $gPoint = $dao->get_point_list($conn, $rs->fields['member_seqno']);

    if($count->fields['cnt'] > 0 ){
        $count_text = "완료(".$count->fields['cnt'].")";
    }else{
        $count_text = "미완료"; 
    }

    $member = $nimdao->selectEmpl($conn, $info->fields['con_man']);

	$point = $dao->get_point_sum($conn,$rs->fields['member_seqno']);
    $rs2 = $dao->selectsumpoint($conn, "SEQ", $param);
    $list2 = selectPointCheck($conn, $dao, $rs2, $param);
   $test = explode("♪",$list2);
	// 현재 전체 포인트 내역 가져오기
	$point_sum = $point['sum_point'];
    if($rs->fields['chk_yn'] == "N" || $rs->fields['chk_yn'] == "Y" ){
        $memberchk = "없음";
    }else{
        $memberchk = $rs->fields['chk_yn'];
    }

        $rs_html .= sprintf($html, $class, 
            $i ,
            $rs->fields['member_name'],
            $rs->fields['refund_name'],
            $rs->fields['tel_num']." /<br>".$rs->fields['cell_num'],
            $gPoint->fields[po_point],
            $gPoint->fields[po_datetime],
            $count_text,
            $test[0],
            $test[1],
            $test[0]-$test[1],
            $memberchk,
            $rs->fields['member_seqno'],
            $rs->fields['member_seqno']);
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}


function makeCounselHtml($conn, $dao, $rs, $param) {

    $nimdao = new NimdaCommonDAO();
	
    if (!$rs) {
        return false;
    }
	

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
	$chkDate = "";
    $chkid = [];
    $users = ['dpuser32', 'dpuser34', 'dpuser38', 'dpuser39', 'dpuser46'];
    $usersName = ['성기환', '이동호', '이다영', '정지선', '윤세한'];
    
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        $member = $nimdao->selectEmpl($conn, $rs->fields['con_man']);

            
            if($chkDate == "" ){
                $test = "test3";
            }else if ($chkDate != $rs->fields['con_date']){
                $test = "test2";
            }else{
                $test = "test3";
            }
if($param["man_list"] == "") {
            if($test == "test2" ){
                
                    $diff1 = array_diff($users, $chkid);
                    $diff2 = array_diff($chkid, $users);
                    $date2 = array_merge($diff1, $diff2);

                    foreach($date2 as $date2_value){
                        $member = $nimdao->selectEmpl($conn, $date2_value);
                        $rs_html .= sprintf($html, $class, 
                        "" ,
                        $member->fields['name'],
                        $chkDate,
                        "",
                        "없음"); 

                        //$i--;
                    }
                
                $chkid = [];
            }
}
            $chkDate = $rs->fields['con_date'];
            array_push($chkid,$rs->fields['con_man'] );

            $rs_html .= sprintf($html, $class, 
                $i ,
                $member->fields['name'],
                $rs->fields['con_date'],
                $rs->fields['member_name'],
               $rs->fields['con_text']); 
            //배열 초기화 
            $date2 = "";
            
           
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}
/*
function getArrayDifference(array $array1, array $array2): array {
    // 첫 번째 배열에서 두 번째 배열에 없는 값
    $diff1 = array_diff($array1, $array2);

    // 두 번째 배열에서 첫 번째 배열에 없는 값
    $diff2 = array_diff($array2, $array1);

    // 두 결과를 합쳐 차이가 나는 모든 값 반환
    return array_merge($diff1, $diff2);
}
*/
function testpointsum($conn, $dao, $rs, $param) {
	
    if (!$rs) {
        return false;
    }
	
    $point_sum = 0;

   while ($rs && !$rs->EOF) {
        $point_sum += number_format($rs->fields["ae_point"]);
        $rs->moveNext();
    }

    return  $point_sum;
}

?>