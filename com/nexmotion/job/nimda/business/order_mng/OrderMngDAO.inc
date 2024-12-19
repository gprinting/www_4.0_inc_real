<?
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * 영업 - 주문관리DAO
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/04/18 엄준현 생성
 *=============================================================================
 */
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');

class OrderMngDAO extends BusinessCommonDAO {
    /**
     * @brief 사용 운영체제/프로그램 정보 검색
     *
     * @param $conn        = connection identifier
     * @param $param       = 검색조건 파라미터
     * @param $fields_name = 가져올 컬럼명
     *
     * @return option html, 운영체제(fields_name = oper_sys일 때만 사용)
     */
    function selectProTypInfo($conn, $param, $field_name = "oper_sys") {
        $rs = $this->selectProTyp($conn, $param, $field_name);

        $oper_sys = $rs->fields["oper_sys"];

        $temp = array();
        $temp["flag"] = 'Y';
        $temp["def"] = "전체";
        $temp["def_val"] = '';
        $temp["dvs"] = $field_name;

        return array(
            "oper_sys" => $oper_sys,
            "html"     => makeSelectOptionHtml($rs, $temp)
        );
    }

    /**
     * @brief 회원 기간별 일별 집계데이터 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDaySalesStatsList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* OrderMngDAO.inc - 영업-매출거래현황정보 리스트 검색시 사용 */";
        $query .= "\n        A.sales_price";
        $query .= "\n       ,A.sale_price";
        $query .= "\n       ,A.net_sales_price";
        $query .= "\n       ,A.depo_price";
        $query .= "\n       ,A.period_end_oa";
        $query .= "\n       ,A.carryforward_oa";
        $query .= "\n       ,A.input_date";
        $query .= "\n  FROM  day_sales_stats AS A";
        $query .= "\n WHERE  member_seqno = %s";
        if ($this->blankParameterCheck($param, "input_date")) {
            $query .= "\n    AND  A.input_date = ";
            $query .= $param["input_date"];
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . $param["from"];
            $query .= " <= A.input_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.input_date <= ";
            $query .= $param["to"];
        }
        $query .= "\n ORDER  BY input_date DESC";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 회원 기간별 일별 집계데이터 개별 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDaySalesStatsEach($conn, $param, $dvs) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* 영업-매출거래현황정보 차트 데이터 생성시 사용 */";
        switch ($dvs) {
            case "sum_oa" :
                $query .= "\n        A.period_end_oa + A.carryforward_oa AS ";
                break;
            case "sum_sale" :
                $query .= "\n        SUM(A.sale_price) AS ";
                break;
            case "sum_depo" :
                $query .= "\n        SUM(A.depo_price) AS ";
                break;
            case "sum_net" :
            case "year_sum_net" :
                $query .= "\n        SUM(A.net_sales_price) AS ";
                break;
        }
        $query .= $dvs;

        $query .= "\n  FROM  day_sales_stats AS A";
        $query .= "\n WHERE  member_seqno = %s";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . $param["from"];
            $query .= " <= A.input_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.input_date <= ";
            $query .= $param["to"];
        }
        $query .= "\n ORDER  BY input_date DESC";
        if ($dvs === "sum_oa") {
            $query .= "\n LIMIT  1";
        }

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields[$dvs];
    }

    /**
     * @brief 전체 회원 수 검색
     *
     * @param $conn = connection identifier
     *
     * @return 검색결과
     */
    function selectMemberCount($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* 영업-영업관리 업체검색리스트에서 전체 업체 수 검색 쿼리*/";
        $query .= "\n        COUNT(1) AS cnt";
        $query .= "\n   FROM member AS A";

        $rs = $conn->Execute($query);

        return $rs->fields["cnt"];
    }

    /**
     * @brief 미수금 판별한 회원번호 검색
     *
     * @param $conn = connection identifier
     * 
     * @return 회원번호
     */
    function selectMemberHasOa($conn, $param, $page, $oa_yn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n SELECT  /* 미수금 판별한 회원번호 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         DISTINCT"; 
        $query .= "\n         A.member_seqno"; 

        $query .= "\n   FROM  day_sales_stats AS A"; 
        $query .= "\n  WHERE  1 = 1"; 
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . $param["from"];
            $query .= " <= A.input_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.input_date <= ";
            $query .= $param["to"];
        }

        if ($oa_yn == "Y") {
            $query .= "\n    AND  (IFNULL(A.period_end_oa, 0) + IFNULL(A.carryforward_oa, 0)) > 0";
        } else if ($oa_yn == "N") {
            $query .= "\n    AND  (IFNULL(A.period_end_oa, 0) + IFNULL(A.carryforward_oa, 0)) = 0";
        } else {
        }

        $query .= "\n ORDER  BY A.member_seqno ASC";

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 회원 담당자에서 회원으로 join해서 회원 일련번호 검색
     * @modified by montvert(17.07.24) - 파라미터 이름 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = limit 시작값
     *
     * @return 검색결과
     */
    function selectMemberMng2Member($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = array("member_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* 영업-영업관리 업체검색리스트에서 회원 일련번호 검색용 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.member_seqno";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.office_nick";
        $query .= "\n   FROM  member AS A";
        if ($this->blankParameterCheck($param, "depar") ||
                $this->blankParameterCheck($param, "empl")) {
            $query .= "\n        ,member_mng AS B";
        }
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "depar") ||
                $this->blankParameterCheck($param, "empl")) {
            $query .= "\n    AND  A.member_seqno = B.member_seqno";
        }
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n    AND  B.resp_deparcode = ";
            $query .= $param["depar"];
        }
        if ($this->blankParameterCheck($param, "empl")) {
            $query .= "\n    AND  (B.ibm = ";
            $query .= $param["empl"];
            $query .= "\n    OR  B.mac = ";
            $query .= $param["empl"];
            $query .= "\n    ) ";
        }
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n    AND  A.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "grade")) {
            $query .= "\n    AND  A.grade = ";
            $query .= $param["grade"];
        }
        if ($this->blankParameterCheck($param, "dlvr_dvs")) {
            $query .= "\n    AND  A.dlvr_dvs = ";
            $query .= $param["dlvr_dvs"];
        }
        if ($this->blankParameterCheck($param, "dlvr_code")) {
            $query .= "\n    AND  A.dlvr_code = ";
            $query .= $param["dlvr_code"];
        }
        if ($this->blankParameterCheck($param, "office_nick")) {
            $query .= "\n    AND  A.office_nick LIKE '%";
            $query .= substr($param["office_nick"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_tel")) {
            $query .= "\n    AND  A.tel_num LIKE '%";
            $query .= substr($param["member_tel"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_cell")) {
            $query .= "\n    AND  A.cell_num LIKE '%";
            $query .= substr($param["member_cell"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_addr")) {
            $query .= "\n    AND  CONCAT(A.addr, ' ', A.addr_detail) LIKE '%";
            $query .= substr($param["member_addr"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno IN (";
            $query .= $param["member_seqno"] .")";
        }
        $query .= "\n  ORDER  BY A.member_seqno";
        $query .= "\n  LIMIT  " . $page . ", 5";
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문 공통에서 제목과 날짜 범위로 검색 후 회원 일련번호만 반환
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = limit 시작값
     *
     * @return 검색결과
     */
    function selectOrderCommon2Member($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = array("member_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* 영업-영업관리 업체검색리스트에서 회원 일련번호 검색용 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         DISTINCT";
        $query .= "\n         B.member_seqno";
        $query .= "\n        ,B.member_name";
        $query .= "\n        ,B.office_nick";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n        ,member AS B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.title LIKE '%%%s%%'";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.order_regi_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.order_regi_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno IN (";
            $query .= $param["member_seqno"] .')';
        }
        $query .= "\n  ORDER  BY B.member_seqno";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $query  = sprintf($query, substr($param["title"], 1, -1));
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문 배송에서 수신자 검색 후 주문공통에서
     * 날짜 범위로 검색 후 회원 일련번호만 반환
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = limit 시작값
     *
     * @return 검색결과
     */
    function selectOrderDlvr2Member($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = array("member_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* 영업-영업관리 업체검색리스트에서 회원 일련번호 검색용 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         DISTINCT";
        $query .= "\n         C.member_seqno";
        $query .= "\n        ,C.member_name";
        $query .= "\n        ,C.office_nick";
        $query .= "\n   FROM  order_dlvr AS A";
        $query .= "\n        ,order_common AS B";
        $query .= "\n        ,member AS C";
        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  B.member_seqno = C.member_seqno";
        $query .= "\n    AND  A.tsrs_dvs = '수신'";
        $query .= "\n    AND  A.name LIKE '%%%s%%'";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= B.order_regi_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  B.order_regi_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno IN (";
            $query .= $param["member_seqno"] .')';
        }
        $query .= "\n  ORDER  BY C.member_seqno";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $query  = sprintf($query, substr($param["receiver"], 1, -1));
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 가상계좌번호로 검색해서 회원 일련번호만 반환
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = limit 시작값
     *
     * @return 검색결과
     */
    function selectVirtBaAdmin2Member($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = array("member_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* 영업-영업관리 업체검색리스트에서 회원 일련번호 검색용 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         DISTINCT";
        $query .= "\n         B.member_seqno";
        $query .= "\n        ,B.member_name";
        $query .= "\n        ,B.office_nick";
        $query .= "\n   FROM  virt_ba_admin AS A";
        $query .= "\n        ,member AS B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.ba_num LIKE '%%%s%%'";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno IN (";
            $query .= $param["member_seqno"] .')';
        }
        $query .= "\n  ORDER  BY B.member_seqno";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $query  = sprintf($query, substr($param["virt_ba"], 1, -1));
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 회원 등급 시작금액 검색
     *
     * @detail 총미수액 - 입금액 까지
     *
     * @param $conn  = connection identifier
     * @param $grade = 회원 등급
     *
     * @return 검색결과
     */
    function selectMemberGradePolicy($conn, $grade) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $grade = $this->parameterEscape($conn, $grade);

        $query  = "\n SELECT  /* 영업-주문관리 검색회원정보-상단-등급부족금액 처리용 쿼리 */";
        $query .= "\n         A.sales_start_price";
        $query .= "\n   FROM  member_grade_policy AS A";
        $query .= "\n  WHERE  A.grade = %s";

        $query  = sprintf($query, $grade);
        //echo $query;

        $rs = $conn->Execute($query);

        return intval($rs->fields["sales_start_price"]);
    }

    /**
     * @brief 검색회원정보애서 일반적인 회원 정보 검색
     *
     * @detail 회원명 - 최근주문일 까지
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리 검색회원정보 검색용 쿼리 */";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.office_nick";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        #작업자번호?";
        $query .= "\n        ,B.tel_num AS accting_tel";
        $query .= "\n        ,B.cell_num AS accting_cell";
        $query .= "\n        ,A.member_typ";
        $query .= "\n        ,A.grade";
        $query .= "\n        ,A.cumul_sales_price";
        $query .= "\n        ,A.first_join_date";
        $query .= "\n        ,A.final_order_date";
        $query .= "\n   FROM  member AS A";
        $query .= "\n   LEFT OUTER JOIN accting_mng AS B";
        $query .= "\n     ON A.member_seqno = B.member_seqno";
        $query .= "\n  WHERE A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 검색회원정보애서 회원 정보 수정
     *
     * @detail 결재종류 - 회원닉네임
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* 영업-주문관리 회원 정보수정용 쿼리 */";
        $query .= "\n         member";
        $query .= "\n    SET  member_seqno = member_seqno";
        if ($this->blankParameterCheck($param, "office_nick")) {
            $query .= "\n        ,office_nick = ";
            $query .= $param["office_nick"];
        }
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
    }

    /**
     * @brief 검색회원정보애서 예외회원 정보 수정
     *
     * @detail 결재종류 - 여신한도소지금액
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateExcptMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* 영업-주문관리 예외회원 정보수정용 쿼리 */";
        $query .= "\n         excpt_member";
        $query .= "\n    SET  member_seqno = member_seqno";
        if ($this->blankParameterCheck($param, "loan_limit_price")) {
            $query .= "\n        ,loan_limit_price = ";
            $query .= $param["loan_limit_price"];
        }
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
    }

    /**
     * @brief 사내닉네임으로 회원정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberByOfficeNick($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-명세서출력 사내닉네임으로 회원 검색 쿼리 */";
        $query .= "\n         A.member_seqno";
        //$query .= "\n        ,A.member_name";
        $query .= "\n        ,A.office_nick";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  A.office_nick LIKE '%%%s%%'";

        $query  = sprintf($query, substr($param["office_nick"], 1, -1));

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 해당 회원에 해당하는 주문내역 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommon($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-명세서출력 사내닉네임으로 회원 검색 쿼리 */";
        $query .= "\n         A.depo_finish_date";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.order_detail";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.sell_price";
        $query .= "\n        ,A.grade_sale_price";
        //$query .= "\n        ,A.member_sale_price";
        $query .= "\n        ,A.use_point_price";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.depo_finish_date BETWEEN %s AND %s";
        $query .= "\n    AND  A.order_state != '1180'";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["from"]
                                , $param["to"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고라 대/중/소분류 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 카테고리 소분류 배열(Key = sortcode, Val = cate_name)
     */
    function selectCateInfoArr($conn, $cate_level = '3') {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"] = "sortcode, cate_name";
        $temp["table"] = "cate";
        $temp["where"]["cate_level"] = $cate_level;

        $rs = $this->selectData($conn, $temp);

        $ret = array();
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;
            $ret[$fields["sortcode"]] = $fields["cate_name"];

            $rs->MoveNext();
        }

        return $ret;
    }

    /**
     * @brief 해당 회원에 해당하는 주문정보내역 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = ["order_num" => true];
        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  CASE WHEN A.dvs = '매출증가' then D.cate_sortcode ELSE '' END cate_sortcode ";
        $query .= "\n         ,CASE WHEN A.dvs = '매출증가' then D.order_num WHEN A.dvs = '배송비' then CONCAT(D.order_num,' 배송비')  ELSE A.member_pay_history_seqno END order_num";
        $query .= "\n        ,(IFNULL(A.card_pay_price,0) + IFNULL(A.pay_price,0) -  IFNULL(A.adjust_price,0) +  IFNULL(A.depo_price,0)) pay_price";
        $query .= "\n        ,CONCAT(IFNULL(D.order_state,''), IFNULL(E.input_dvs,'')) order_state";
        $query .= "\n        ,CONCAT(IFNULL(D.title,''), ' ',IFNULL(E.cont,''), CASE  WHEN A.depo_price != 0 then '선입금'  ELSE '' END) title";
        $query .= "\n        ,D.order_detail";
        $query .= "\n        ,CONCAT(IFNULL(C.dlvr_way,''), IFNULL(E.input_dvs_detail,'')) dlvr_way, D.pay_way";
        $query .= "\n        ,CONCAT(IFNULL(D.cust_memo,''), IFNULL(E.input_dvs_detail,'')) cust_memo";
        $query .= "\n        , IFNULL(A.card_depo_price,0) card_depo_price,IFNULL(A.card_pay_price,0) card_pay_price";
        $query .= "\n        , IFNULL(A.adjust_price,0) adjust_price, IFNULL(A.depo_price,0) depo_price";
        $query .= "\n        ,E.cont";
        $query .= "\n        ,A.sale_price";
        $query .= "\n   FROM member_pay_history AS A ";
        $query .= "\n   INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n   LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n   LEFT JOIN order_dlvr AS C ON D.order_common_seqno = C.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n   LEFT JOIN adjust AS E ON A.adjust_seqno = E.adjust_seqno ";

        $query .= "\n  WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date IS NULL ";
        $query .= "\n    AND  A.member_seqno = %s AND A.depo_price = 0";
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  A.cate_sortcode LIKE '";
            $query .= substr($param["cate_sortcode"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.deal_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.deal_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        if ($this->blankParameterCheck($param, "order_num")) {
            $query .= "\n    AND  A.order_num IN (";
            $query .= $param["order_num"] . ')';
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  D.title LIKE '%%";
            $query .= substr($param["title"], 1, -1) . "%%'";
        }
        $query .= "\n    HAVING pay_price != 0 OR card_pay_price != 0 ";
        $query  = sprintf ($query, $param["member_seqno"]);

        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 영업-주문관리-주문상세정보 중 기본정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoBase($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-주문상세정보 중 기본정보 */";
        $query .= "\n         A.order_common_seqno";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.page_cnt";
        $query .= "\n        ,A.mono_yn";
        $query .= "\n        ,A.sell_price";
        $query .= "\n        ,A.add_after_price";
        $query .= "\n        ,A.add_opt_price";
        $query .= "\n        ,A.grade_sale_price";
        $query .= "\n        ,A.member_sale_price";
        $query .= "\n        ,A.use_point_price";
        //$query .= "\n        ,A.cp_price";
        //$query .= "\n        ,A.event_price";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n  WHERE  A.order_num = %s";

        $query  = sprintf($query, $param["order_num"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 영업-주문관리-주문상세정보 중 낱장형 상세정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoDetailSheet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-주문상세정보 낱장형 상세정보 */";
        $query .= "\n         A.order_detail_dvs_num";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.stan_name";
        $query .= "\n        ,A.print_tmpt_name";
        $query .= "\n        ,A.tot_tmpt";
        $query .= "\n        ,CONCAT(B.name, ' ', B.dvs, ' ', B.color, ' ', B.basisweight) AS paper_name";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.page_amt";
        $query .= "\n        ,A.paper_price";
        $query .= "\n        ,A.output_price";
        $query .= "\n        ,A.print_price";
        $query .= "\n        ,A.paper_sum_price";
        $query .= "\n        ,A.output_sum_price";
        $query .= "\n        ,A.print_sum_price";
        $query .= "\n   FROM  order_detail AS A";
        $query .= "\n        ,cate_paper   AS B";
        $query .= "\n  WHERE  A.order_common_seqno = %s";
        $query .= "\n    AND  A.cate_paper_mpcode = B.mpcode";

        $query  = sprintf($query, $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 영업-주문관리-주문상세정보 중 책자형 상세정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoDetailBrochure($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-주문상세정보 책자형 상세정보 */";
        $query .= "\n         A.order_detail_dvs_num";
        $query .= "\n        ,A.typ";
        $query .= "\n        ,A.stan_name";
        $query .= "\n        ,A.print_tmpt_name";
        $query .= "\n        ,A.tot_tmpt";
        $query .= "\n        ,CONCAT(B.name, ' ', B.dvs, ' ', B.color, ' ', B.basisweight) AS paper_name";
        $query .= "\n        ,A.paper_price";
        $query .= "\n        ,A.output_price";
        $query .= "\n        ,A.print_price";
        $query .= "\n        ,A.paper_sum_price";
        $query .= "\n        ,A.output_sum_price";
        $query .= "\n        ,A.print_sum_price";
        $query .= "\n   FROM  order_detail_brochure AS A";
        $query .= "\n        ,cate_paper   AS B";
        $query .= "\n  WHERE  A.order_common_seqno = %s";
        $query .= "\n    AND  A.cate_paper_mpcode = B.mpcode";

        $query  = sprintf($query, $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 상세별로 속한 후공정 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-주문상세정보 상세별 후공정 검색*/";
        $query .= "\n         A.after_name";
        $query .= "\n        ,A.price";
        $query .= "\n   FROM  order_after_history AS A";
        $query .= "\n  WHERE  A.order_detail_dvs_num = %s";

        $query  = sprintf($query, $param["order_detail_dvs_num"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 상세별로 속한 옵션 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoOption($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리-주문상세정보 옵션 검색*/";
        $query .= "\n         A.opt_name";
        $query .= "\n        ,A.price";
        $query .= "\n   FROM  order_opt_history AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief CRM정보(영업) 문자메시지 총합
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 문자메시지 총합
     */
    function selectCrmBusinessMessageTotal($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - CRM관리(영업) - 문자건수총합 */";
        $query .= "\n         SUM(A.msg_cnt)        AS msg_total_cnt";

        $query .= "\n   FROM  crm_biz_info AS A";
        $query .= "\n  WHERE  1 = 1 ";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "crm_info_depar")) {
            $query .= "\n    AND  A.empl_name IN (";
            $query .= $param["crm_info_depar"] .")";
            if ($this->blankParameterCheck($param, "crm_info_empl")) {
                $query .= "\n    AND  A.empl_name = ";
                $query .= $param["crm_info_empl"];
            }
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE ";
            $query .= "CONCAT('%%'," . $param["member_name"] . ",'%%')";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.cs_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.cs_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }


        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief CRM정보(수금) 문자메시지 총합
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 문자메시지 총합
     */
    function selectCrmCollectMessageTotal($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - CRM관리(수금) - 문자건수총합 */";
        $query .= "\n         SUM(A.msg_cnt)        AS msg_total_cnt";

        $query .= "\n   FROM  crm_collect_info AS A";
        $query .= "\n  WHERE  1 = 1 ";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "crm_info_depar")) {
            $query .= "\n    AND  A.empl_name IN (";
            $query .= $param["crm_info_depar"] .")";
            if ($this->blankParameterCheck($param, "crm_info_empl")) {
                $query .= "\n    AND  A.empl_name = ";
                $query .= $param["crm_info_empl"];
            }
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE ";
            $query .= "CONCAT('%%'," . $param["member_name"] . ",'%%')";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.cs_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.cs_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }


        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief CRM정보 리스트 검색
     *
     * @detail 커넥션 정보 따로 잡아야됨
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = limit 시작값
     *
     * @return 검색결과
     */
    function selectCrmInfoList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 리스트 검색 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         B.cIDX        AS seqno";
        $query .= "\n        ,B.cModifyDate AS date";
        $query .= "\n        ,B.cName       AS member_name";
        $query .= "\n        ,C.cNAME       AS crm_typ";
        $query .= "\n        ,B.cUserID     AS empl_name";
        $query .= "\n        ,B.cCallMemo   AS memo";
        $query .= "\n   FROM  tbl_center                AS A";
        $query .= "\n        ,tbl_campaign_data_history AS B";
        $query .= "\n        ,tbl_code                  AS C";
        $query .= "\n  WHERE  A.cCenterID   = B.cSite";
        $query .= "\n    AND  B.cSite       = C.cSite";
        $query .= "\n    AND  B.cOutcome2   = C.cSMALLCD";
        if ($this->blankParameterCheck($param, "depar_name")) {
            $query .= "\n    AND  A.cCenterName = ";
            $query .= $param["depar_name"];
        }
        if ($this->blankParameterCheck($param, "empl_name")) {
            $query .= "\n    AND  B.cUserID = ";
            $query .= $param["empl_name"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  B.cName = '%%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%%'";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= B.cModifyDate";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  B.cModifyDate <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  LIMIT  " . $page . ", 5";

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 CRM정보 일자별 집계 리스트 검색
     *
     * @detail 커넥션 정보 따로 잡아야됨
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     * 2017.06.05 이청산 : 참조하는 테이블이 바뀌어 주석처리함
     */
   /* function selectCrmInfoSumList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
   */
   //   $query  = "\n SELECT  /* CRM정보 리스트 검색 쿼리 */";
   /*   $query .= "\n         DATE(B.cModifyDate) AS date";
        $query .= "\n        ,COUNT(*)            AS cnt";
        $query .= "\n        ,B.cName             AS member_name";
        $query .= "\n        ,B.cUserID           AS empl_name";
        $query .= "\n   FROM  tbl_center                AS A";
        $query .= "\n        ,tbl_campaign_data_history AS B";
        $query .= "\n        ,tbl_code                  AS C";
        $query .= "\n  WHERE  A.cCenterID   = B.cSite";
        $query .= "\n    AND  B.cSite       = C.cSite";
        $query .= "\n    AND  B.cOutcome2   = C.cSMALLCD";
        if ($this->blankParameterCheck($param, "depar_name")) {
            $query .= "\n    AND  A.cCenterName = ";
            $query .= $param["depar_name"];
        }
        if ($this->blankParameterCheck($param, "empl_name")) {
            $query .= "\n    AND  B.cUserID = ";
            $query .= $param["empl_name"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  B.cName = ";
            $query .= $param["member_name"];
        }
        if ($this->blankParameterCheck($param, "member_name_like")) {
            $query .= "\n    AND  B.cName LIKE '%";
            $query .= substr($param["member_name_like"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= B.cModifyDate";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  B.cModifyDate <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  GROUP BY DATE(B.cModifyDate), B.cName";
        $query .= "\n  ORDER BY B.cIdx DESC";
        //echo $query;

        return $conn->Execute($query);
    } */  

    /**
     * @brief 선택한 회원의 CRM정보 집계리스트 불러오기
     * 
     * @writer montvert(2017.06.05)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoSumList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 검색 쿼리 */";
        $query .= "\n         A.crm_collect_info_seqno as seqno";
        $query .= "\n       , A.cs_date";
        $query .= "\n       , B.member_name";
        $query .= "\n       , A.cs_typ";
        $query .= "\n       , A.empl_name";
        $query .= "\n       , A.loan_pay_promi_date";
        $query .= "\n       , A.memo";
        $query .= "\n   FROM  crm_collect_info A";
        $query .= "\n        ,member B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.member_seqno = ";
        $query .= $param["member_seqno"];
        if ($this->blankParameterCheck($param, "empl_name")) {
            $query .= "\n    AND  A.empl_name = ";
            $query .= $param["empl_name"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  B.member_name = ";
            $query .= $param["member_name"];
        }
        if ($this->blankParameterCheck($param, "member_name_like")) {
            $query .= "\n    AND  B.member_name LIKE '%";
            $query .= substr($param["member_name_like"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.cs_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.cs_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  ORDER BY A.cs_date DESC";
        $query .= "\n  LIMIT  " . $page . ", 5";

        return $conn->Execute($query);

    } 

    /**
     * @brief CRM 영업 문자건수 갱신
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateCrmBusinessMmsCount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 영업 문자건수 갱신 쿼리 */";
        $query .= "\n         crm_biz_info";
        $query .= "\n    SET  msg_cnt = msg_cnt + 1";
        $query .= "\n  WHERE  crm_biz_info_seqno = %s";

        $query  = sprintf($query, $param["seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
    }

    /**
     * @brief CRM 수금 문자건수 갱신
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateCrmCollectMmsCount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 영업 문자건수 갱신 쿼리 */";
        $query .= "\n         crm_collect_info";
        $query .= "\n    SET  msg_cnt = msg_cnt + 1";
        $query .= "\n  WHERE  crm_collect_info_seqno = %s";

        $query  = sprintf($query, $param["seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
    }
   
    /**
     * @brief 선택한 회원의 CRM정보 영업정보 불러오기
     * 
     * @writer      montvert(2017.05.26)
     * @modified by montvert(2017.07.13)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoBusiness($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 검색 쿼리 */";
        $query .= "\n         cs_date";
        $query .= "\n       , cs_indu";
        $query .= "\n       , cs_promi_date";
        $query .= "\n       , cs_type";
        $query .= "\n       , interest_field";
        $query .= "\n       , interest_prdt";
        $query .= "\n       , expec_sales";
        $query .= "\n       , interest_item";
        $query .= "\n       , plural_deal_yn";
        $query .= "\n       , cs_cont";
        $query .= "\n       , cs_memo";
        $query .= "\n       , empl_name";
        $query .= "\n       , member_seqno";
        $query .= "\n   FROM  crm_biz_info";
        $query .= "\n  WHERE  crm_biz_info_seqno = ";
        $query .= $param["crm_biz_info_seqno"];

        return $conn->Execute($query);

    } 

    /**
     * @brief 선택한 회원의 CRM정보 영업정보 중 직원기념일
     *
     * @writer montvert(2017.05.26)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmEmplAnniv($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 직원기념일 */";
        $query .= "\n         crm_biz_info_empl_seqno";
        $query .= "\n        ,cont";
        $query .= "\n        ,empl_anniv";
        $query .= "\n   FROM  crm_biz_info_empl";
        $query .= "\n  WHERE  member_seqno = ";
        $query .= $param["member_seqno"];

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 영업정보 중 복수거래기업
     *
     * @writer montvert(2017.05.30)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmEtprsName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 복수거래기업 */";
        $query .= "\n         crm_biz_info_etprs_seqno";
        $query .= "\n        ,etprs_name";
        $query .= "\n   FROM  crm_biz_info_etprs";
        $query .= "\n  WHERE  crm_biz_info_seqno = ";
        $query .= $param["crm_biz_info_seqno"];

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 영업정보 등록
     *
     * @writer montvert(2017.05.26)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertCrmInfoBusiness($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_biz_info (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,member_name";
        $query .= "\n       ,cs_date";
        $query .= "\n       ,cs_indu";
        $query .= "\n       ,cs_promi_date";
        $query .= "\n       ,cs_type";
        $query .= "\n       ,interest_field";
        $query .= "\n       ,interest_prdt";
        $query .= "\n       ,expec_sales";
        $query .= "\n       ,interest_item";
        $query .= "\n       ,plural_deal_yn";
        $query .= "\n       ,cs_cont";
        $query .= "\n       ,cs_memo";
        $query .= "\n       ,empl_name";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";
        $query  = sprintf($query, $param["member_seqno"]
                                , $param["member_name"] 
                                , $param["cs_date"] 
                                , $param["cs_indu"] 
                                , $param["cs_promi_date"] 
                                , $param["cs_type"] 
                                , $param["interest_field"] 
                                , $param["interest_prdt"] 
                                , $param["expec_sales"] 
                                , $param["interest_item"] 
                                , $param["plural_deal_yn"] 
                                , $param["cs_cont"]
                                , $param["cs_memo"]
                                , $param["empl_name"]);

        return $conn->Execute($query);

    }

    /**
     * @brief CRM 영업정보리스트
     *
     * @writer montvert(2017.07.14)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoBusinessList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $excpt_arr = array("crm_info_depar" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* CRM정보 영업탭 리스트 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.crm_biz_info_seqno";
        $query .= "\n        ,A.cs_date";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.cs_indu";
        $query .= "\n        ,A.empl_name";
        $query .= "\n        ,A.cs_promi_date";
        $query .= "\n        ,A.cs_cont";
        $query .= "\n        ,A.msg_cnt";
        $query .= "\n   FROM  crm_biz_info A";
        $query .= "\n  WHERE  1 = 1 ";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "crm_info_depar")) {
            $query .= "\n    AND  A.empl_name IN (";
            $query .= $param["crm_info_depar"] .")";
            if ($this->blankParameterCheck($param, "crm_info_empl")) {
                $query .= "\n    AND  A.empl_name = ";
                $query .= $param["crm_info_empl"];
            }
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE ";
            $query .= "CONCAT('%%'," . $param["member_name"] . ",'%%')";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.cs_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.cs_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  ORDER BY A.crm_biz_info_seqno DESC";
        $query .= "\n  LIMIT ". $page .", 5";
        return $conn->Execute($query);

    }

    /**
     * @brief CRM정보 직원정보
     *
     * @writer montvert(2017.08.28)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmDeparEmpl($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        $query  = "\n SELECT  /* CRM정보 직원정보 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.name";
        $query .= "\n   FROM  empl AS A";
        $query .= "\n  WHERE  A.depar_code = ";
        $query .= $param;

        return $conn->Execute($query);

    }
    

    /**
     * @brief CRM 영업정보 회원정보
     *
     * @writer montvert(2017.07.17)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoBusinessMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 회원정보*/";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.cell_num";
        $query .= "\n   FROM  member A";
        $query .= "\n        ,crm_biz_info B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno ";
        $query .= "\n    AND  B.crm_biz_info_seqno = %s";
        $query  = sprintf($query, $param["crm_biz_seqno"]);
        
        return $conn->Execute($query);

    }

    /**
     * @brief CRM 수금정보 회원정보
     *
     * @writer montvert(2017.07.21)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoCollectMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 회원정보*/";
        $query .= "\n         A.member_name";
        $query .= "\n        ,A.cell_num";
        $query .= "\n   FROM  member A";
        $query .= "\n        ,crm_collect_info B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno ";
        $query .= "\n    AND  B.crm_collect_info_seqno = %s";
        $query  = sprintf($query, $param["crm_col_seqno"]);
        
        return $conn->Execute($query);

    }


    /**
     * @brief 선택한 회원의 CRM정보 영업정보 수정
     *
     * @writer montvert(2017.07.17)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateCrmInfoBusiness($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 영업탭 업데이트 쿼리 */";
        $query .= "\n         crm_biz_info";
        $query .= "\n    SET  member_seqno = member_seqno";
        if ($this->blankParameterCheck($param, "cs_date")) {
            $query .= "\n        ,cs_date = ";
            $query .= $param["cs_date"];
        }
        if ($this->blankParameterCheck($param, "cs_indu")) {
            $query .= "\n        ,cs_indu = ";
            $query .= $param["cs_indu"];
        }
        if ($this->blankParameterCheck($param, "cs_promi_date")) {
            $query .= "\n        ,cs_promi_date = ";
            $query .= $param["cs_promi_date"];
        }
        if ($this->blankParameterCheck($param, "cs_type")) {
            $query .= "\n        ,cs_type = ";
            $query .= $param["cs_type"];
        }
        if ($this->blankParameterCheck($param, "interest_field")) {
            $query .= "\n        ,interest_field = ";
            $query .= $param["interest_field"];
        }
        if ($this->blankParameterCheck($param, "interest_prdt")) {
            $query .= "\n        ,interest_prdt = ";
            $query .= $param["interest_prdt"];
        }
        if ($this->blankParameterCheck($param, "expec_sales")) {
            $query .= "\n        ,expec_sales = ";
            $query .= $param["expec_sales"];
        }
        if ($this->blankParameterCheck($param, "interest_item")) {
            $query .= "\n        ,interest_item = ";
            $query .= $param["interest_item"];
        }
        if ($this->blankParameterCheck($param, "plural_deal_yn")) {
            $query .= "\n        ,plural_deal_yn = ";
            $query .= $param["plural_deal_yn"];
        }
        if ($this->blankParameterCheck($param, "cs_cont")) {
            $query .= "\n        ,cs_cont = ";
            $query .= $param["cs_cont"];
        }
        if ($this->blankParameterCheck($param, "cs_memo")) {
            $query .= "\n        ,cs_memo = ";
            $query .= $param["cs_memo"];
        }
        $query .= "\n  WHERE  crm_biz_info_seqno = %s";
        $query  = sprintf($query, $param["crm_biz_info_seqno"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 수금정보 수정
     *
     * @writer montvert(2017.07.21)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateCrmInfoCollect($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 수금탭 업데이트 쿼리 */";
        $query .= "\n         crm_collect_info";
        $query .= "\n    SET  member_seqno = member_seqno";
        $query .= "\n       , modi_date = now()";
        if ($this->blankParameterCheck($param, "memo")) {
            $query .= "\n        ,memo = ";
            $query .= $param["memo"];
        }
        if ($this->blankParameterCheck($param, "loan_pay_promi_dvs")) {
            $query .= "\n        ,loan_pay_promi_dvs = ";
            $query .= $param["loan_pay_promi_dvs"];
        }
        if ($this->blankParameterCheck($param, "loan_pay_promi_date")) {
            $query .= "\n        ,loan_pay_promi_date = ";
            $query .= $param["loan_pay_promi_date"];
        }
        if ($this->blankParameterCheck($param, "loan_pay_promi_price")) {
            $query .= "\n        ,loan_pay_promi_price = ";
            $query .= $param["loan_pay_promi_price"];
        }
        $query .= "\n  WHERE  crm_collect_info_seqno = %s";
        $query  = sprintf($query, $param["crm_collect_info_seqno"]);

        return $conn->Execute($query);

    }


    /**
     * @brief CRM 수금정보리스트
     *
     * @writer montvert(2017.07.20)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoCollectList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $excpt_arr = array("crm_info_depar" => true);

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n SELECT  /* CRM정보 수금탭 리스트 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.crm_collect_info_seqno";
        $query .= "\n        ,A.member_seqno";
        $query .= "\n        ,A.cs_date";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.empl_name";
        $query .= "\n        ,A.loan_pay_promi_date";
        $query .= "\n        ,A.memo";
        $query .= "\n        ,A.msg_cnt";
        $query .= "\n   FROM  crm_collect_info A";
        $query .= "\n  WHERE  1 = 1 ";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "crm_info_depar")) {
            $query .= "\n    AND  A.empl_name IN (";
            $query .= $param["crm_info_depar"] .")";
            if ($this->blankParameterCheck($param, "crm_info_empl")) {
                $query .= "\n    AND  A.empl_name = ";
                $query .= $param["crm_info_empl"];
            }
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE ";
            $query .= "CONCAT('%%'," . $param["member_name"] . ",'%%')";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.cs_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.cs_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  ORDER BY A.crm_collect_info_seqno DESC";
        $query .= "\n  LIMIT ". $page .", 5";
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 중 직원 기념일 등록/수정
     * 지우고 다시 등록하는 식이다.
     *
     * @writer montvert(2017.05.29)
     * @comment 현재 사용되지 않음
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     *//*
    function insertCrmInfoEmplAnniv($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_biz_info_empl (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,cont";
        $query .= "\n       ,empl_anniv";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["cont"]
                                , $param["empl_anniv"]);
        return $conn->Execute($query);

    }*/

    /**
     * @brief 선택한 회원의 CRM메모 삭제(영업)
     *
     * @writer montvert(2017.07.19)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function deleteCrmBusinessMemoBySeqno($conn, $memo_seq) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $query  = "\n DELETE FROM crm_biz_info_memo";
        $query .= "\n       WHERE crm_biz_info_memo_seqno = ";
        $query .= $memo_seq;

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 CRM메모 삭제(수금)
     *
     * @writer montvert(2017.07.21)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function deleteCrmCollectMemoBySeqno($conn, $memo_seq) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $query  = "\n DELETE FROM crm_collect_info_memo";
        $query .= "\n       WHERE crm_collect_info_memo_seqno = ";
        $query .= $memo_seq;

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 CRM정보 중 복수거래 추가
     *
     * @writer montvert(2017.05.29)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertCrmInfoEtprsName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_biz_info_etprs (";
        $query .= "\n        crm_biz_info_seqno";
        $query .= "\n       ,etprs_name";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["crm_biz_info_seqno"]
                                , $param["etprs_name"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 메모 추가(영업)
     *
     * @writer montvert(2017.07.18)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertCrmMemoBusiness($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_biz_info_memo (";
        $query .= "\n        crm_biz_info_seqno";
        $query .= "\n       ,memo_date";
        $query .= "\n       ,memo_cont";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["crm_biz_info_seqno"]
                                , $param["memo_date"]
                                , $param["memo_cont"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 메모 수정(영업)
     *
     * @writer montvert(2017.07.19)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateCrmMemoBusiness($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 영업탭 메모 업데이트 쿼리 */";
        $query .= "\n         crm_biz_info_memo";
        $query .= "\n    SET  crm_biz_info_memo_seqno = crm_biz_info_memo_seqno";
        if ($this->blankParameterCheck($param, "memo_date")) {
            $query .= "\n        ,memo_date = ";
            $query .= $param["memo_date"];
        }
        if ($this->blankParameterCheck($param, "memo_cont")) {
            $query .= "\n        ,memo_cont = ";
            $query .= $param["memo_cont"];
        }
        $query .= "\n  WHERE  crm_biz_info_memo_seqno = %s";
        $query  = sprintf($query, $param["memo_seqno"]);

        return $conn->Execute($query);

    }


    /**
     * @brief 선택한 회원의 CRM정보 메모 추가(수금) 
     *
     * @writer montvert(2017.07.18)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertCrmMemoCollect($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_collect_info_memo (";
        $query .= "\n        crm_collect_info_seqno";
        $query .= "\n       ,memo_date";
        $query .= "\n       ,memo_cont";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["crm_collect_info_seqno"]
                                , $param["memo_date"]
                                , $param["memo_cont"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 메모 수정(수금)
     *
     * @writer montvert(2017.07.21)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateCrmMemoCollect($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 수금탭 메모 업데이트 쿼리 */";
        $query .= "\n         crm_collect_info_memo";
        $query .= "\n    SET  crm_collect_info_memo_seqno = crm_collect_info_memo_seqno";
        if ($this->blankParameterCheck($param, "memo_date")) {
            $query .= "\n        ,memo_date = ";
            $query .= $param["memo_date"];
        }
        if ($this->blankParameterCheck($param, "memo_cont")) {
            $query .= "\n        ,memo_cont = ";
            $query .= $param["memo_cont"];
        }
        $query .= "\n  WHERE  crm_collect_info_memo_seqno = %s";
        $query  = sprintf($query, $param["memo_seqno"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 중 복수거래업체 삭제
     *
     * @writer montvert(2017.05.29)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function deleteCrmInfoEtprsName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);
        
        $query  = "\n DELETE FROM crm_biz_info_etprs";
        $query .= "\n       WHERE crm_biz_info_seqno = ";
        $query .= $param["crm_biz_info_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 품목별 현황정보 카테고리별 건수, 결제금액 집계
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtInfoSum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-품목별 현황정보 집계정보 검색 쿼리 */";
        $query .= "\n         A.cate_top";
        $query .= "\n        ,sum(A.count) AS cnt";
        $query .= "\n        ,sum(A.pay_price) AS sum_pay";
        $query .= "\n   FROM  day_cate_sales_stats AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . $param["from"];
            $query .= "  <= A.input_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.input_date <= ";
            $query .= $param["to"];
        }
        $query .= "\n  GROUP BY A.cate_top";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["from"]
                                , $param["to"]);
        return $conn->Execute($query);
    }

    /**
     * @brief 문자보내기 데이터입력
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
     function insertMms($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO MMS_MSG (";
        $query .= "\n        subject";
        $query .= "\n       ,phone";
        $query .= "\n       ,callback";
        $query .= "\n       ,reqdate";
        $query .= "\n       ,msg";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["subject"]
                                , $param["phone"]
                                , $param["callback"]
                                , $param["msg"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM정보 수금정보 불러오기
     * 
     * @writer montvert(2017.06.01)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoBank($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 수금탭 검색 쿼리 */";
        $query .= "\n         bank_name";
        $query .= "\n       , ba_num";
     // $query .= "\n       , state";
        $query .= "\n   FROM  virt_ba_admin";
        $query .= "\n  WHERE  member_seqno = ";
        $query .= $param["member_seqno"];

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 CRM 메모 리스트 불러오기(영업)
     * 
     * @writer montvert(2017.07.18)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmBusinessMemoList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 영업탭 메모 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         crm_biz_info_memo_seqno";
        $query .= "\n       , memo_date";
        $query .= "\n       , memo_cont";
        $query .= "\n   FROM  crm_biz_info_memo";
        $query .= "\n  WHERE  crm_biz_info_seqno = ";
        $query .= $param["crm_biz_info_seqno"];
        $query .= "\n  ORDER BY  crm_biz_info_memo_seqno DESC";
        $query .= "\n  LIMIT  ". $page . ", 5";

        return $conn->Execute($query);

    } 

    /**
     * @brief CRM정보 수정용 메모내용 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmBusinessMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM영업 메모 검색(수정용)*/";
        $query .= "\n         A.memo_date";
        $query .= "\n        ,A.memo_cont";
        $query .= "\n   FROM  crm_biz_info_memo AS A";
        $query .= "\n  WHERE  A.crm_biz_info_memo_seqno = %s";

        $query  = sprintf($query, $param["memo_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 CRM 메모 리스트 불러오기(수금)
     * 
     * @writer montvert(2017.07.18)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmCollectMemoList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 수금탭 메모 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         crm_collect_info_memo_seqno";
        $query .= "\n       , memo_date";
        $query .= "\n       , memo_cont";
        $query .= "\n   FROM  crm_collect_info_memo";
        $query .= "\n  WHERE  crm_collect_info_seqno = ";
        $query .= $param["crm_collect_info_seqno"];
        $query .= "\n  ORDER BY  crm_collect_info_memo_seqno DESC";
        $query .= "\n  LIMIT  ". $page . ", 5";

        return $conn->Execute($query);

    } 

    /**
     * @brief CRM정보 수정용 메모내용 검색(수금)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmCollectMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM수금 메모 검색(수정용)*/";
        $query .= "\n         A.memo_date";
        $query .= "\n        ,A.memo_cont";
        $query .= "\n   FROM  crm_collect_info_memo AS A";
        $query .= "\n  WHERE  A.crm_collect_info_memo_seqno = %s";

        $query  = sprintf($query, $param["memo_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 CRM정보 여신정보 불러오기
     * 
     * @writer montvert(2017.06.01)
     * @modify montvert(2017.07.21) 미사용 정보 주석처리 
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoExcpt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 여신한도 검색 쿼리 */";
        $query .= "\n         loan_limit_price";
     // $query .= "\n        ,loan_pay_promi_dvs";
     // $query .= "\n        ,loan_pay_promi_date";
     // $query .= "\n        ,loan_pay_promi_price";
        $query .= "\n   FROM  excpt_member";
        $query .= "\n  WHERE  member_seqno = ";
       	$query .= $param["member_seqno"]; 

        return $conn->Execute($query);

    } 
    
    /**
     * @brief 선택한 회원의 CRM정보 미수액 불러오기
     * 
     * @writer montvert(2017.07.21)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 미수액 검색 쿼리 */";
        $query .= "\n         period_end_oa";
        $query .= "\n        ,carryforward_oa";
        $query .= "\n   FROM  day_sales_stats";
        $query .= "\n  WHERE  member_seqno = ";
       	$query .= $param["member_seqno"]; 
        $query .= "\n  ORDER BY  input_date DESC";
        $query .= "\n  LIMIT  1";

        return $conn->Execute($query);

    } 
    /**
     * @brief 선택한 회원의 CRM정보 한도 소진금액 불러오기
     * 
     * @writer montvert(2017.06.01)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoLack($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM정보 한도 소진금액 검색 쿼리 */";
        $query .= "\n         order_lack_price";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = ";
        $query .= $param["member_seqno"];

        return $conn->Execute($query);

    } 

    /**
     * @brief 선택한 회원의 CRM정보 수금정보 새로 입력
     *
     * @writer montvert(2017.06.05)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertCrmInfoCollectInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO crm_collect_info (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,member_name";
        $query .= "\n       ,cs_date";
        $query .= "\n       ,modi_date";
        $query .= "\n       ,empl_name";
        $query .= "\n       ,memo";
        $query .= "\n       ,loan_pay_promi_dvs";
        $query .= "\n       ,loan_pay_promi_date";
        $query .= "\n       ,loan_pay_promi_price";
        $query .= "\n       ,loan_limit_price";
        $query .= "\n       ,loan_limit_use";
        $query .= "\n       ,handle_dvs";
        $query .= "\n       ,handle_date";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["member_name"]
                                , $param["empl_name"]
                                , $param["memo"]
                                , $param["loan_pay_promi_dvs"]
                                , $param["loan_pay_promi_date"]
                                , $param["loan_pay_promi_price"]
                                , $param["loan_limit"]
                                , $param["loan_lack"]
                                , $param["handle_dvs"]
                                , $param["handle_date"]);

        return $conn->Execute($query);

    }
    /**
     * @brief 사용자의 CRM 기본정보 업데이트 
     * 
     * @writer montvert(2017.06.07)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateCrmInfoExcptMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* CRM 기본정보 업데이트 쿼리 */";
        $query .= "\n         excpt_member";
        $query .= "\n    SET  member_seqno = member_seqno";
        if ($this->blankParameterCheck($param, "loan_pay_promi_dvs")) {
            $query .= "\n        ,loan_collect_dvs = ";
            $query .= $param["loan_pay_promi_dvs"];
        }
        if ($this->blankParameterCheck($param, "loan_pay_promi_date")) {
            $query .= "\n        ,loan_pay_promi_date = ";
            $query .= $param["loan_pay_promi_date"];
        }
        if ($this->blankParameterCheck($param, "loan_pay_promi_price")) {
            $query .= "\n        ,loan_pay_promi_price = ";
            $query .= $param["loan_pay_promi_price"];
        }
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
        
    }


    /**
     * @brief 선택한 리스트의 CRM 세부정보 불러오기
     * 
     * @writer montvert(2017.06.07)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmInfoDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* CRM 세부정보 검색 쿼리 */";
        $query .= "\n         loan_pay_promi_dvs";
        $query .= "\n        ,loan_pay_promi_date";
        $query .= "\n        ,loan_pay_promi_price";
        $query .= "\n        ,memo";
        $query .= "\n        ,empl_name";
        $query .= "\n        ,cs_date";
        $query .= "\n        ,loan_limit_price";
        $query .= "\n        ,loan_limit_use";
        $query .= "\n        ,handle_dvs";
        $query .= "\n        ,handle_date";
        $query .= "\n   FROM  crm_collect_info";
        $query .= "\n  WHERE  crm_collect_info_seqno = ";
       	$query .= $param["crm_collect_info_seqno"]; 

        return $conn->Execute($query);

    } 

    /**
     * @brief CRM정보 기간별 집계데이터 검색
     *
     * @writer montvert(2017.07.24)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCrmSalesStatsList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* 영업-매출거래현황정보 리스트 검색시 사용 */";
        $query .= "\n        A.net_sales_price";
        $query .= "\n       ,A.depo_price";
        $query .= "\n       ,A.input_date";
        $query .= "\n  FROM  day_sales_stats AS A";
        $query .= "\n WHERE  member_seqno = %s";
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . $param["from"];
            $query .= " <= A.input_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.input_date <= ";
            $query .= $param["to"];
        }
        $query .= "\n ORDER  BY input_date DESC";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }


    /**
     * @brief 회원정보 확인(일반회원, 예외회원)
     * 
     * @writer montvert(2017.06.08)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function checkExcptMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 회원정보 확인 쿼리 */";
        $query .= "\n         member_typ";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = ";
       	$query .= $param["member_seqno"]; 

        return $conn->Execute($query);

    }

    /**
     * @brief 매출정보 세부정보 확인
     * 
     * @writer montvert(2017.06.28)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectSalesDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출정보 세부정보 확인 쿼리 */";
        $query .= "\n         A.cate_sortcode";
        $query .= "\n        ,A.order_num";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.order_detail";
        $query .= "\n        ,A.order_state";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.pay_price";
        $query .= "\n        ,B.dlvr_way";
        $query .= "\n        ,A.cust_memo";

        $query .= "\n   FROM  order_common A";
        $query .= "\n        ,order_dlvr   B";

        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  B.tsrs_dvs = '수신'";
        $query .= "\n    AND  A.member_seqno = %s";
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  A.cate_sortcode LIKE '";
            $query .= substr($param["cate_sortcode"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "searchTxt")) {
            $query .= "\n    AND  A.title LIKE ";
            $query .= "CONCAT('%%'," . $param["searchTxt"] . ",'%%')";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.depo_finish_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.depo_finish_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }

        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고리 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectMidCateList($conn, $sortcode = null) {
        $param = array();
        $param["col"]   = "sortcode, cate_name";
        $param["table"] = "cate";
        $param["where"]["cate_level"] = "2";

        $rs = $this->selectData($conn, $param);

        $basic_option = "중분류(전체)";

        if (strlen($sortcode) === 6) {
            $basic_option = "소분류(전체)";
        }

        return makeOptionHtml($rs, "sortcode", "cate_name", $basic_option);
    }

    /**
     * @brief 판번호로 주문번호 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 주문번호
     */
    function selectSalesSheetTypset($conn, $param) {
 
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 판번호로 주문번호 검색 쿼리 */";
        $query .= "\n         B.order_num";

        $query .= "\n   FROM  sheet_typset A";
        $query .= "\n        ,amt_order_detail_sheet B";

        $query .= "\n  WHERE  A.sheet_typset_seqno = B.sheet_typset_seqno";
        if ($this->blankParameterCheck($param, "searchTxt")) {
            $query .= "\n    AND  A.typset_num LIKE ";
            $query .= "CONCAT('%'," . $param["searchTxt"] . ",'%')";
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 판번호로 주문번호 검색(브로셔)
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 주문번호
     */
    function selectSalesBrochureTypset($conn, $param) {
 
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 판번호로 주문번호 검색 쿼리(브로셔) */";
        $query .= "\n         B.order_detail_dvs_num";

        $query .= "\n   FROM  brochure_typset A";
        $query .= "\n        ,page_order_detail_brochure B";

        $query .= "\n  WHERE  A.brochure_typset_seqno = B.brochure_typset_seqno";
        if ($this->blankParameterCheck($param, "searchTxt")) {
            $query .= "\n    AND  A.typset_num LIKE ";
            $query .= "CONCAT('%%'," . $param["searchTxt"] . ",'%%')";
        }
        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문번호로 매출상세 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectSalesOrderDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출정보 세부정보 확인 쿼리 */";
        $query .= "\n         A.cate_sortcode";
        $query .= "\n        ,A.order_num";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.order_detail";
        $query .= "\n        ,A.order_state";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.pay_price";
        $query .= "\n        ,B.dlvr_way";
        $query .= "\n        ,A.cust_memo";

        $query .= "\n   FROM  order_common A";
        $query .= "\n        ,order_dlvr   B";

        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  B.tsrs_dvs = '수신'";
        $query .= "\n    AND  A.order_num = %s";
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  A.cate_sortcode LIKE '";
            $query .= substr($param["cate_sortcode"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.depo_finish_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.depo_finish_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query  = sprintf ($query, $param["dvs_num"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문번호로 매출상세 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectMemberPrepayPrice($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $temp = array();
        $temp["col"] = "prepay_price";
        $temp["table"] = "member";
        $temp["where"]["member_seqno"] = $member_seqno;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["prepay_price"];
    }

    /**
     * @brief 선택한 회원의 생산투입한도설정
     *
     * @writer montvert(2017.07.04)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertManuLimit($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO produce_limit_set (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,regi_date";
        $query .= "\n       ,limit_price";
        $query .= "\n       ,depo_promi_date";
        $query .= "\n       ,regi_empl";
        $query .= "\n       ,limit_cate";
        $query .= "\n       ,memo";
        $query .= "\n       ,release_empl";
        $query .= "\n       ,depo_yn";
        $query .= "\n       ,deal_date";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["member_seqno"]
                                , "now()"
                                , $param["limit_price"]
                                , $param["depo_promi_date"]
                                , $param["regi_empl"]
                                , $param["limit_cate"]
                                , $param["memo"]
                                , $param["release_empl"]
                                , $param["depo_yn"]
                                , $param["deal_date"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 선택한 회원의 생산투입한도 로드(엑셀다운로드용)
     *
     * @writer montvert(2017.07.05)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return list
     */
    function selectManuLimitForExcel($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출정보 세부정보 확인 쿼리 */";
        $query .= "\n         A.member_name";
        $query .= "\n        ,B.regi_date";
        $query .= "\n        ,B.limit_price";
        $query .= "\n        ,B.deal_date";
        $query .= "\n        ,B.depo_promi_date";
        $query .= "\n        ,B.regi_empl";
        $query .= "\n        ,B.limit_cate";
        $query .= "\n        ,B.memo";
        $query .= "\n        ,B.release_empl";
        $query .= "\n        ,B.depo_yn";

        $query .= "\n   FROM  member A";
        $query .= "\n        ,produce_limit_set B";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  B.member_seqno = %s";
        $query .= "\n  ORDER BY B.regi_date DESC";
        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }
    /**
     * @brief 선택한 회원의 생산투입한도 로드
     *
     * @writer montvert(2017.07.05)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return list
     */
    function selectManuLimit($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출정보 세부정보 확인 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.member_name";
        $query .= "\n        ,B.regi_date";
        $query .= "\n        ,B.limit_price";
        $query .= "\n        ,B.deal_date";
        $query .= "\n        ,B.depo_promi_date";
        $query .= "\n        ,B.regi_empl";
        $query .= "\n        ,B.limit_cate";
        $query .= "\n        ,B.memo";
        $query .= "\n        ,B.release_empl";
        $query .= "\n        ,B.depo_yn";

        $query .= "\n   FROM  member A";
        $query .= "\n        ,produce_limit_set B";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  B.member_seqno = %s";
        $query .= "\n  ORDER BY B.regi_date DESC";
        $query .= "\n  LIMIT  " . $page . ", 5";
        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 선택한 회원의 기존 잔고 로드
     *
     * @writer montvert(2017.07.07)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function selectPrevBal($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출정보 세부정보 확인 쿼리 */";
        $query .= "\n         A.prepay_bal";

        $query .= "\n   FROM  member_pay_history A";

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n  ORDER BY A.member_pay_history_seqno DESC";
        $query .= "\n  LIMIT  1";
        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields["prepay_bal"];
    }

    /**
     * @brief 매출액, 입금액 리스트 로드
     *
     * @writer montvert(2017.07.07)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function selectSalesDepo($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출액, 입금액 리스트 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.deal_date";
        $query .= "\n       , A.order_num";
        $query .= "\n       , A.cont";
        $query .= "\n       , A.dvs";
        $query .= "\n       , A.order_cancel_yn";
        $query .= "\n       , A.pay_price";
        $query .= "\n       , A.depo_price";
        $query .= "\n       , A.input_typ";
        $query .= "\n       , A.prepay_bal";

        $query .= "\n   FROM  member_pay_history A";

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n  ORDER BY A.member_pay_history_seqno DESC";
        $query .= "\n  LIMIT  " . $page .", 5";
        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 선택한 회원의 매출액, 입금액 설정
     *
     * @writer montvert(2017.07.06)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertSalesDepo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_pay_history (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,deal_date";
        $query .= "\n       ,dvs";
        $query .= "\n       ,input_typ";
        $query .= "\n       ,cont";
        $query .= "\n       ,pay_price";
        $query .= "\n       ,depo_price";
        $query .= "\n       ,exist_prepay";
        $query .= "\n       ,prepay_bal";
        $query .= "\n       ,dvs_detail";
        $query .= "\n       ,empl_name";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["deal_date"]
                                , $param["dvs_input"]
                                , $param["input_typ"]
                                , $param["cont"]
                                , $param["pay_price"]
                                , $param["depo_price"]
                                , $param["pre_bal"]
                                , $param["total"]
                                , $param["cont_detail"]
                                , $param["empl_name"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 매출액, 입금액 설정(세부정보입력)
     *
     * @writer montvert
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertSalesDepoDetailInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_pay_history_card (";
        $query .= "\n        member_pay_history_seqno";
        $query .= "\n       ,card_num";
        $query .= "\n       ,mip_mon";
        $query .= "\n       ,aprvl_num";
        $query .= "\n       ,aprvl_date";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["history_seqno"]
                                , $param["card_num"]
                                , $param["mip_mon"]
                                , $param["aprvl_num"]
                                , $param["aprvl_date"]);
        return $conn->Execute($query);

    }
    
    /**
     * @brief 매출액,입금액 설정에서 member테이블과 싱크
     *
     * @param $conn  = connection identifier
     * @param $param = 수정값 파라미터
     *
     * @return 수정결과
     */
    function updateSalesDepoToMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* 매출액,입금액 잔고 싱크용 쿼리 */";
        $query .= "\n         member";
        $query .= "\n    SET  member_seqno = member_seqno";
        if ($this->blankParameterCheck($param, "total")) {
            $query .= "\n        ,prepay_price = ";
            $query .= $param["total"];
        }
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return ($rs === false) ? false : true;
    }

    /**
     * @brief 매출액 VIEW
     * @writer montvert
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function selectDepoViewData($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 매출액 VIEW 리스트 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.deal_date";
        $query .= "\n       , A.pay_price";
        $query .= "\n       , A.depo_price";
        $query .= "\n       , A.input_typ";
        $query .= "\n       , C.member_name";
        $query .= "\n       , B.card_num";
        $query .= "\n       , B.mip_mon";
        $query .= "\n       , B.aprvl_num";

        $query .= "\n   FROM  member_pay_history A";
        $query .= "\n   LEFT JOIN member_pay_history_card B";
        $query .= "\n          ON A.member_pay_history_seqno = B.member_pay_history_seqno";
        $query .= "\n   LEFT JOIN member C";
        $query .= "\n          ON A.member_seqno = C.member_seqno";

        $query .= "\n  WHERE  A.member_seqno = %s";
        if ($this->blankParameterCheck($param, "input_typ")) {
            $query .= "\n    AND  A.input_typ = ";
            $query .= $param["input_typ"];
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.deal_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.deal_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        $query .= "\n  ORDER BY A.member_pay_history_seqno DESC";
        $query .= "\n  LIMIT  " . $page .", 5";
        $query  = sprintf ($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 에누리액 VIEW
     * @writer montvert
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function selectDiscountViewData($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 에누리액 VIEW 리스트 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.deal_date";
        $query .= "\n       , A.order_num";
        $query .= "\n       , A.depo_price";
        $query .= "\n       , A.cont";

        $query .= "\n   FROM  member_pay_history A";

        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.input_typ LIKE CONCAT(%s,'%%')";
        
        $query .= "\n  ORDER BY A.member_pay_history_seqno DESC";
        $query .= "\n  LIMIT  " . $page .", 5";
        $query  = sprintf ($query, $param["member_seqno"]
                                 , $param["depo_dvs"]);

        $rs = $conn->Execute($query);

        return $rs;
    }
    
    /**
     * @brief 에누리액 입력
     *
     * @writer montvert(2017.07.12)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function insertDiscountViewData($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_pay_history (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,deal_date";
        $query .= "\n       ,input_typ";
        $query .= "\n       ,cont";
        $query .= "\n       ,pay_price";
        $query .= "\n       ,depo_price";
        $query .= "\n       ,exist_prepay";
        $query .= "\n       ,prepay_bal";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,now() ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_typ"]
                                , $param["cont"]
                                , $param["pay_price"]
                                , $param["depo_price"]
                                , $param["pre_bal"]
                                , $param["total"]);
        return $conn->Execute($query);

    }



    /**
     * @brief 주문상태에 따른 주문리스트 검색
     * @detail 카테고리별로 합산되서 나옴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function selectTotalOrderPriceByStatus($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - 주문관리 - 주문진행정보(가격총합) */";
        $query .= "\n         SUM(B.sell_price)        AS sell_price";
        $query .= "\n        ,SUM(B.grade_sale_price)  AS grade_sale_price";
        $query .= "\n        ,SUM(B.member_sale_price) AS member_sale_price";
        $query .= "\n        ,SUM(B.cp_price)          AS cp_price";
        $query .= "\n        ,SUM(B.use_point_price)   AS use_point_price";
        $query .= "\n        ,SUM(C.dlvr_price)        AS dlvr_price";
        $query .= "\n        ,SUM(B.pay_price)         AS pay_price";

        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,order_common AS B";

        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n        ,member_mng AS D";
        }

        $query .= "\n LEFT OUTER JOIN order_dlvr AS C";
        $query .= "\n              ON B.order_common_seqno = C.order_common_seqno";
        $query .= "\n             AND  C.tsrs_dvs = '수신'";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n    AND  A.member_seqno = D.member_seqno";
            $query .= "\n    AND  B.member_seqno = D.member_seqno";
            $query .= "\n    AND  D.resp_deparcode = ";
            $query .= $param["depar"];
        }
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n    AND  A.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_state")) {
            $query .= "\n    AND  B.order_state = ";
            $query .= $param["order_state"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  B.cate_sortcode LIKE ";
            $query .= substr($param["cate_sortcode"], 0, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  B.title LIKE '%";
            $query .= substr($param["title"], 0, -1);
            $query .= "%'";
        }
        $query .= "\n    AND  B.order_regi_date BETWEEN ";
        $query .= substr($param["from"], 0, -1) . " 00:00:00' AND ";
        $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 주문상태에 따른 주문리스트 검색
     * @detail 카테고리별로 합산되서 나옴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return
     */
    function selectOrderCommonSumByStatus($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - 주문관리 - 주문진행정보 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         COUNT(A.member_seqno) AS order_cnt";
        $query .= "\n        ,A.member_seqno";
        $query .= "\n        ,B.order_regi_date";
        $query .= "\n        ,B.receipt_start_date";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,B.title";
        $query .= "\n        ,B.order_detail";
        $query .= "\n        ,CONCAT(SUM(B.amt), B.amt_unit_dvs) AS amt";
        $query .= "\n        ,SUM(B.count) AS count";
        $query .= "\n        ,SUM(B.sell_price)        AS sell_price";
        $query .= "\n        ,SUM(B.grade_sale_price)  AS grade_sale_price";
        $query .= "\n        ,SUM(B.member_sale_price) AS member_sale_price";
        $query .= "\n        ,SUM(B.cp_price)          AS cp_price";
        $query .= "\n        ,SUM(B.use_point_price)   AS use_point_price";
        //$query .= "\n        ,SUM(C.dlvr_price)        AS dlvr_price";
        $query .= "\n        ,SUM(B.pay_price)         AS pay_price";
        $query .= "\n        ,C.dlvr_way";
        $query .= "\n        ,C.invo_cpn";

        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,order_common AS B";
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n        ,member_mng AS D";
        }

        $query .= "\n LEFT OUTER JOIN order_dlvr AS C";
        $query .= "\n              ON B.order_common_seqno = C.order_common_seqno";
        $query .= "\n             AND  C.tsrs_dvs = '수신'";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n    AND  A.member_seqno = D.member_seqno";
            $query .= "\n    AND  B.member_seqno = D.member_seqno";
            $query .= "\n    AND  D.resp_deparcode = ";
            $query .= $param["depar"];
        }
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n    AND  A.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_state")) {
            $query .= "\n    AND  B.order_state = ";
            $query .= $param["order_state"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  B.cate_sortcode LIKE ";
            $query .= substr($param["cate_sortcode"], 0, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  B.title LIKE '%";
            $query .= substr($param["title"], 0, -1);
            $query .= "%'";
        }
        $query .= "\n    AND  B.order_regi_date BETWEEN ";
        $query .= substr($param["from"], 0, -1) . " 00:00:00' AND ";
        $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        $query .= "\n  GROUP BY A.member_seqno";
        $query .= "\n  LIMIT " . $page . ", 10";
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문상태에 따른 주문리스트 검색
     * @detail 회원별로 합산되서 나옴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommonByStatus($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - 주문관리 - 주문진행정보(상세보기 팝업) */";
        $query .= "\n         B.order_regi_date";
        $query .= "\n        ,B.receipt_start_date";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.cell_num";
        $query .= "\n        ,B.title";
        $query .= "\n        ,B.order_detail";
        $query .= "\n        ,CONCAT(B.amt, B.amt_unit_dvs) AS amt";
        $query .= "\n        ,B.count";
        $query .= "\n        ,B.sell_price";
        $query .= "\n        ,B.grade_sale_price";
        $query .= "\n        ,B.member_sale_price";
        $query .= "\n        ,B.cp_price";
        $query .= "\n        ,B.use_point_price";
        $query .= "\n        ,B.add_after_price";
        $query .= "\n        ,B.add_opt_price";
        $query .= "\n        ,B.pay_price";

        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,order_common AS B";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.member_seqno = %s";
        $query .= "\n    AND  B.order_state  = %s ";
        $query .= "\n    AND  B.order_regi_date BETWEEN ";
        $query .= "'%s 00:00:00' AND '%s 23:59:59'";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["order_state"]
                                , substr($param["from"], 1, -1)
                                , substr($param["to"], 1, -1));

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문상태에 따른 주문 수, 금액합계 검색
     * @detail 회원별로 합산되서 나옴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommonCountByStatus($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업 - 주문관리 - 주문진행정보(상세보기 주문수, 금액합계) */";
        $query .= "\n         SUM(B.count) AS count";
        $query .= "\n        ,SUM(B.sell_price)        AS sell_price";
        $query .= "\n        ,SUM(B.grade_sale_price)  AS grade_sale_price";
        $query .= "\n        ,SUM(B.member_sale_price) AS member_sale_price";
        $query .= "\n        ,SUM(B.cp_price)          AS cp_price";
        $query .= "\n        ,SUM(B.use_point_price)   AS use_point_price";
        //$query .= "\n        ,SUM(C.dlvr_price)        AS dlvr_price";
        $query .= "\n        ,SUM(B.pay_price)         AS pay_price";

        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,order_common AS B";
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n        ,member_mng AS D";
        }

        $query .= "\n LEFT OUTER JOIN order_dlvr AS C";
        $query .= "\n              ON B.order_common_seqno = C.order_common_seqno";
        $query .= "\n             AND  C.tsrs_dvs = '수신'";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param, "depar")) {
            $query .= "\n    AND  A.member_seqno = D.member_seqno";
            $query .= "\n    AND  B.member_seqno = D.member_seqno";
            $query .= "\n    AND  D.resp_deparcode = ";
            $query .= $param["depar"];
        }
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n    AND  A.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_state")) {
            $query .= "\n    AND  B.order_state = ";
            $query .= $param["order_state"];
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  A.member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  B.cate_sortcode LIKE ";
            $query .= substr($param["cate_sortcode"], 0, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  B.title LIKE '%";
            $query .= substr($param["title"], 0, -1);
            $query .= "%'";
        }
        $query .= "\n    AND  B.order_regi_date BETWEEN ";
        $query .= substr($param["from"], 0, -1) . " 00:00:00' AND ";
        $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        $query .= "\n  GROUP BY A.member_seqno";
        //echo $query;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문관리-신규회원정보리스트 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDeparAdminList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }

        $temp = [];
        $temp["col"] = "depar_code, depar_name";
        $temp["table"] = "depar_admin";
        $temp["where"]["cpn_admin_seqno"] = $param["cpn_admin_seqno"];

        return $this->selectData($conn, $temp);
    }

    /**
     * @brief 주문관리-신규회원정보리스트 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectNewMemberList($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  /* OrderMngDAO.inc 영업-주문관리-신규회원정보 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.member_seqno";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.member_typ";
        $query .= "\n        ,A.grade";
        $query .= "\n        ,A.first_join_date";
        $query .= "\n        ,A.first_order_date";
        $query .= "\n        ,B.resp_deparcode";
        $query .= "\n        ,A.dlvr_code";
        $query .= "\n        ,C.name AS ibm_name";
        $query .= "\n        ,D.name AS mac_name";

        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,member_mng AS B";
        $query .= "\n        ,empl AS C";
        $query .= "\n        ,empl AS D";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  B.ibm = C.empl_seqno";
        $query .= "\n    AND  B.mac = D.empl_seqno";
        $query .= "\n    AND  A.cpn_admin_seqno = %s";
        $query .= "\n    AND  A.first_join_date BETWEEN %s 00:00:00' AND %s 23:59:59'";
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n      AND  A.member_name LIKE '%%";
            $query .= $param["member_name"];
            $query .= "%%'";
        }
        $query .= "\n  LIMIT " . $page . ", 5";

        $query  = sprintf($query, $param["cpn_admin_seqno"]
                                , substr($param["from"], 0, -1)
                                , substr($param["to"], 0, -1));

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문관리-crm관리 집계 선입금
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberPrepayBal($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT A.prepay_bal";
        $query .= "\n   FROM member_pay_history AS A";
        $query .= "\n  WHERE A.member_seqno = %s";
        $query .= "\n    AND A.deal_date BETWEEN %s AND %s";
        $query .= "\n  ORDER BY A.deal_date DESC";
        $query .= "\n  LIMIT 1";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["from"]
                                , $param["to"]);

        $rs = $conn->Execute($query);
        
        return $rs->fields["prepay_bal"];
    }

    /**
     * @brief 주문관리 생산투입한도 등록시 입금대기 주문 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommonDepoWait($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* OrderMngDAO.inc - 입금대기 주문검색 */";
        $query .= "\n         A.order_common_seqno";
        $query .= "\n        ,A.order_lack_price";
        $query .= "\n        ,SUBSTR(A.cate_sortcode, 1, 3) AS cate_top";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";
        $query .= "\n    AND  A.order_state = %s";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["order_state"]);

        $rs = $conn->Execute($query);
        
        return $rs;
    }

    /**
     * @brief 주문관리 생산투입한도 등록시 입금대기 주문 접수대기로 변경
     *
     * @param $conn  = connection identifier
     * @param $param = 수정조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderCommonDepoWait($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE  /* OrderMngDAO.inc - 입금대기 주문 접수대기로 변경 */";
        $query .= "\n         order_common ";
        $query .= "\n    SET  order_state = %s";
        $query .= "\n        ,order_lack_price = '0'";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["order_common_seqno"]);

        $rs = $conn->Execute($query);
        
        return $rs;
    }

    /**
     * @brief 명함/전단출고자 검색
     *
     * @detail 명함/스티커 -> 명함출고팀 / 그 외 -> 전단출고팀
     *
     *
     * @param $conn  = connection identifier
     * @param $param = 수정조건 파라미터
     *
     * @return 검색결과
     */
    function selectNcBlReleaseResp($conn) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }

        $query  = "\n SELECT  /* OrderMngDAO.inc - 생산투입한도 명함/전단출고자 검색 */";
        $query .= "\n         A.depar_name";
        $query .= "\n        ,B.name";
        $query .= "\n   FROM  depar_admin AS A";
        $query .= "\n        ,empl AS B";
        $query .= "\n  WHERE  A.depar_code = B.depar_code";
        $query .= "\n    AND  A.depar_name IN ('명함출고팀','전단출고팀')";
        $query .= "\n    AND  B.job_code = '001'";

        $rs = $conn->CacheExecute(30000, $query);
        $ret = [];

        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $ret[$fields["depar_name"]] = $fields["name"];

            $rs->MoveNext();
        }

        return $ret;
    }

    /**
     * @brief 회원작성 메모내용 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCustMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 회원작성 메모 검색(수정불가)*/";
        $query .= "\n         A.cust_memo";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n  WHERE  A.order_num = %s";

        $query  = sprintf($query, $param["order_num"]);

        return $conn->Execute($query);
    }


}
?>
