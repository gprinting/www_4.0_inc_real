<?
include_once(INC_PATH . '/common_dao/CommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/MakeCommonHtml.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/common/NimdaCommonHtml.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/common/OrderInfoPopDOC.inc');

/*! 공통 DAO Class */
class NimdaCommonDAO extends CommonDAO {
    /**
     * @brief 직원별 페이지 권한 검사
     * @param $conn DB Connection
     * @param $seqno 직원 일련번호
     * @param $path 페이지 경로
     * @return boolean
     */
    function checkAuth($conn, $seqno, $path) {

        if (!$conn) {
            echo "connection failed\n";
            return false;
        }

        $query  = "\n SELECT auth_yn FROM auth_admin_page";
        $query .= "\n  WHERE page_url='" . $path . "'";
        $query .= "\n    AND empl_seqno='" . $seqno . "'";

        $rs = $conn->Execute($query);

        if ($rs->fields["auth_yn"] == "N") {
            echo "<script>";
            echo "    alert('접근 권한이 없습니다.');";
            echo "    history.go(-1);";
            echo "</script>";

        //이것도 'N'일때로 추가해야함
        } else if (!$rs->fields["auth_yn"]) {
            // 주소가 클레임관리 팝업일경우 건너뜀. 많아질경우 로직수정필요
            if ($path ="/business/pop_claim_info.html") {
                return false;
            }
            echo "<script>";
            echo "    alert('접근 권한을 설정해주세요.');";
            echo "    history.go(-1);";
            echo "</script>";
        }
    }

    /**
     * @brief 직원별 페이지 권한 url
     * @param $conn DB Connection
     * @param $seqno 직원 일련번호
     * @param $path 페이지 경로
     * @return boolean
     */
    function selectAuthPage($conn, $param) {

        if (!$conn) {
            echo "connection failed\n";
            return false;
        }

        $query  = "\n SELECT page_url FROM auth_admin_page";
        $query .= "\n  WHERE empl_seqno='" . $param["empl_seqno"] . "'";
        $query .= "\n    AND auth_yn = 'Y'";

        //해당페이지
        if ($this->blankParameterCheck($param ,"section")) {
            $query .= "\n   AND page_url LIKE '%/" . $param["section"] . "/%'";

        }

        $query .= "\n  ORDER BY auth_admin_page_seqno ASC";
        $query .= "\n  LIMIT 1";

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 직원별 페이지 권한 url
     * @param $conn DB Connection
     * @param $seqno 직원 일련번호
     * @param $path 페이지 경로
     * @return boolean
     */
    function selectAuthPage2($conn, $param) {

        if (!$conn) {
            echo "connection failed\n";
            return false;
        }

        $query  = "\n SELECT page_url, auth_yn FROM auth_admin_page";
        $query .= "\n  WHERE empl_seqno='" . $param["empl_seqno"] . "'";
        $query .= "\n  ORDER BY page_url ASC";

        $rs = $conn->Execute($query);

        return $rs;
    }

    function selectChannelInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n    SELECT  * ";
        $query .= "\n      FROM  cpn_admin AS A ";
        $query .= "\n      INNER JOIN member AS B ON A.sell_site = B.sell_channel ";
        $query .= "\n     WHERE  B.member_seqno = '%s'";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 직원 권한 페이지 추가 기본 함수
     * @param $conn DB Connection
     * @param $auth_page 권한 페이지
     * @param $seqno 직원 일련번호
     */
    function addAuthDefault($conn, $auth_page, $seqno) {

        $page_url = "";
        $auth_info = "";

        $high_check = 1;
        $high_cnt = 1;
        $tmp_high_name = "";

        $most_check = 0;
        $most_cnt = 1;
        $tmp_most_name = "";

        foreach ($auth_page as $key => $val) {
            $page_url = $key;
            $auth_info = $val;

            $auth_list = explode("|", $auth_info);
            $most_page_name = $auth_list[0];
            $high_page_name = $auth_list[1];
            $page_name = $auth_list[2];

            if ($tmp_high_name == $high_page_name) {
                $high_check ++;

                //임시 이름과 상위 페이지 이름이 다를 경우
            } else {
                $tmp_high_name = $high_page_name;
                $high_check = 1;
            }

            //임시 이름와 최상위 페이지 이름이 같을 경우
            if ($tmp_most_name == $most_page_name) {

                //임시 이름와 최상위 페이지 이름이 다를 경우
            } else {
                $tmp_most_name = $most_page_name;
                $most_check ++;
            }

            $param["table"] = "auth_admin_page";
            $param["col"]["empl_seqno"] = $seqno;
            $param["col"]["page_url"] = $page_url;
            $param["col"]["most_page_name"] = $most_page_name;
            $param["col"]["high_page_name"] = $high_page_name;
            $param["col"]["page_name"] = $page_name;
            $param["col"]["auth_yn"] = "N";
            $param["col"]["array_num"] = $most_check;
            $param["col"]["detail_array_num"] = $high_check;

            $result = $this->insertData($conn,$param);
        }
    }

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

        return $conn->Execute($query);
    }

    function selectPrepayPriceSql($conn, $id) {

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

        echo $query;
        //return $conn->Execute($query);
    }


    function selectPrepayPriceUntilDate($conn, $id, $date) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $id = $this->parameterEscape($conn, $id);
        $date = $this->parameterEscape($conn, $date);

        $query  = "\n SELECT get_prepay(%s, %s) ";
        /*
        $query  = "\n SELECT prepay_price_money ";
        $query  .= "\n FROM member ";
        $query  .= "\n WHERE id = %s ";
        */
        $query = sprintf($query,
            $id, $date);

        return $conn->Execute($query);
    }

     function selectPrepayPriceUntilDate2($conn, $id, $date) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $id = $this->parameterEscape($conn, $id);
        $date = $this->parameterEscape($conn, $date);

        $query  = "\n SELECT get_prepay(%s, %s) ";
        /*
        $query  = "\n SELECT prepay_price_money ";
        $query  .= "\n FROM member ";
        $query  .= "\n WHERE id = %s ";
        */
        $query = sprintf($query,
            $id, $date);

        echo $query;
    }

    /**
     * @brief 카테고리 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectCateList($conn, $sortcode = null) {
        $param = array();
        $param["col"]   = "sortcode, cate_name";
        $param["table"] = "cate";
        if ($sortcode === null) {
            $param["where"]["cate_level"] = "1";
        } else {
            $param["where"]["high_sortcode"] = $sortcode;
        }

        $rs = $this->selectData($conn, $param);

        $basic_option = "대분류(전체)";

        if (strlen($sortcode) === 3) {
            $basic_option = "중분류(전체)";
        } else if (strlen($sortcode) === 6) {
            $basic_option = "소분류(전체)";
        }

        return makeOptionHtml($rs, "sortcode", "cate_name", $basic_option);
    }

    /**
     * @brief 후가공 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectAftList($conn) {
        $param = array();
        $param["col"]   = "name";
        $param["table"] = "after_dscr";

        $rs = $this->selectData($conn, $param);

        return makeOptionHtml($rs, "name", "name", null, false);
    }

    /**
     * @brief 옵션 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = connection identifier
     *
     * @return 검색결과
     */
    function selectOptList($conn) {
        $param = array();
        $param["col"]   = "name";
        $param["table"] = "opt_dscr";

        $rs = $this->selectData($conn, $param);

        return makeOptionHtml($rs, "name", "name", null, false);
    }

    /**
     * @brief 회사관리 테이블 검색
     *
     * @param $conn  = connection identifier
     *
     * @return option html
     */
    function selectCpnAdmin($conn, $param = array()) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "cpn_admin_seqno, sell_site";
        $temp["table"] = "cpn_admin";
        if ($this->blankParameterCheck($param, "cpn_admin_seqno")) {
            $temp["where"]["cpn_admin_seqno"] = $param["cpn_admin_seqno"];
        }

        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 판매채널 html 생성
     *
     * @param $conn  = connection identifier
     *
     * @return option html
     */
    function selectSellSite($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $rs = $this->selectCpnAdmin($conn);

        return makeOptionHtml($rs, "cpn_admin_seqno", "sell_site", "", "N");
    }

    /**
     * @brief 직원 부서(팀)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDeparInfo($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $val = substr($param["depar_code"], 1, -1);

        $query  = "\nSELECT  A.depar_name ";
        $query .= "\n       ,A.depar_code ";
        $query .= "\n  FROM  depar_admin AS A ";
        $query .= "\n WHERE  (depar_code LIKE '" . $val ."%'";
        $query .= "\n    OR  depar_code = ". $param["depar_code2"] .")";
        $query .= "\n   AND  depar_level = 2";

        //카테고리 분류코드 빈값 체크
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = $param[sell_site] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 접수(영업)팀 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectReceiptDepar($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* 접수팀 검색 공통쿼리  */";
        $query .= "\n        depar_code";
        $query .= "\n       ,depar_name";
        $query .= "\n   FROM depar_admin";
        $query .= "\n  WHERE high_depar_code = '003'";
        $query .= "\n    AND expo_yn = 'Y'";
        $query .= "\n    AND cpn_admin_seqno = " . $param["cpn_admin_seqno"];
        // 나중에 자기 속한 영업팀만 볼 경우 감안해서 주석
        // ca cm cs 별로 처리할건지 부서명으로 처리할건지 로직필요
        //if ($this->blankParameterCheck($param ,"depar_code")) {
        //    $query .= "\n    AND  A.depar_name = " . $param["depar_name"];
        //}

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 일련번호로 주문 내용 팝업 html 생성
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 일련번호
     *
     * @return 주문정보팝업 html
     */
    function selectOrderInfoHtml($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query  = "\n SELECT  prdt_basic_info";
        $query .= "\n        ,prdt_add_info";
        $query .= "\n        ,prdt_price_info";
        $query .= "\n        ,prdt_pay_info";

        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $seqno);

        $rs = $conn->Execute($query);

        return makeOrderInfoHtml($rs->fields);
    }

    /**
     * @brief 입력받은 값으로 사내 닉네임 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    function selectOfficeName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  member_name";
        $query .= "\n        ,member_seqno";
        $query .= "\n        ,office_nick";
        $query .= "\n        ,sell_channel";
        $query .= "\n        ,CONCAT(office_nick) AS full_name";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  1 = 1";

        if ($this->blankParameterCheck($param, "sell_site")) {
            $query .= "\n    AND  sell_channel = " . $param["sell_site"];
        }

        if ($this->blankParameterCheck($param, "office_nick")) {
            $query .= "\n    AND  office_nick LIKE '%";
            $query .= substr($param["office_nick"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        //$query .= "\n    AND  withdraw_dvs = 1";
        $query .= "\n    AND  withdraw_yn = 'N'";

        return $conn->Execute($query);
    }
    	    /**
     * @brief 입력받은 값으로 사내 아이디 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    function selectOfficeID($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  member_name";
        $query .= "\n        ,member_seqno";
        $query .= "\n        ,office_nick";
		$query .= "\n        ,id";
        $query .= "\n        ,sell_channel";
        $query .= "\n        ,CONCAT(id) AS full_name";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  1 = 1";

        if ($this->blankParameterCheck($param, "sell_site")) {
            $query .= "\n    AND  sell_channel = " . $param["sell_site"];
        }

        if ($this->blankParameterCheck($param, "office_nick")) {
            $query .= "\n    AND  office_nick LIKE '%";
            $query .= substr($param["office_nick"], 1, -1);
            $query .= "%'";
        }
        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  member_name LIKE '%";
            $query .= substr($param["member_name"], 1, -1);
            $query .= "%'";
        }
        //$query .= "\n    AND  withdraw_dvs = 1";
        $query .= "\n    AND  withdraw_yn = 'N'";

        return $conn->Execute($query);
    }


    /**
     * @brief 사내 닉네임 list Select
     *
     * @param $conn  = connection identifier
     * @param $param = 검색 조건 파라미터
     *
     * @return : resultSet
     */
    function selectOfficeNickList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    SELECT  member_name";
        $query .= "\n           ,member_seqno";
        $query .= "\n      FROM  member";
        //$query .= "\n     WHERE  cpn_admin_seqno = " . $param["sell_site"];
        $query .= "\n       AND  withdraw_dvs = '1'";

        //사내닉네임
        if ($this->blankParameterCheck($param, "search")) {
            $query .= "\n       AND office_nick LIKE '%";
            $query .= substr($param["search"], 1,-1) . "%'";
        }

        $result = $conn->Execute($query);

        return $result;
    }

     /**
     * @brief 아이디와 비밀번호로 직원 정보 검색
     *
     * @param $conn = connection identifier
     * @param $id   = 직원 id
     */
    function selectEmpl($conn, $id) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $id = $this->parameterEscape($conn, $id);

        $query  = "\nSELECT B.enter_date ";  /* 입사일 */
        $query .= "\n      ,A.passwd ";      /* 비밀번호 */
        $query .= "\n      ,A.empl_seqno ";  /* 일련번호 */
        $query .= "\n      ,A.depar_code ";  /* 직원 부서코드 */
        $query .= "\n      ,B.name ";        /* 직원 이름 */
        $query .= "\n  FROM empl AS A ";
        $query .= "\n      ,empl_human_info AS B ";
        $query .= "\n WHERE A.empl_seqno = B.empl_seqno ";
        $query .= "\n   AND A.empl_id = %s ";

        $query  = sprintf($query, $id);

        return $conn->Execute($query);
    }

    /**
     * @brief 입력받은 값으로 회원명, 사내닉네임
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    function selectCndMember($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  member_name";
        $query .= "\n        ,office_nick";
        $query .= "\n        ,member_id";
        $query .= "\n        ,member_seqno";
        $query .= "\n        ,grade";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  cpn_admin_seqno = $param[sell_site]";

        //조건 검색
        if ($this->blankParameterCheck($param ,"search_txt")) {
            $field = substr($param["search_cnd"], 1, -1);
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  $field LIKE '%$val%' ";
        }
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  member_seqno = ";
            $query .= $param["member_seqno"];
        }

        $query .= "\n    AND  withdraw_dvs = 1";

        return $conn->Execute($query);
    }

    /**
     * @brief 입력받은 값으로 인쇄물제목, 주문번호
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    function selectCndOrder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  title ";
        $query .= "\n        ,order_num ";
        $query .= "\n        ,order_common_seqno ";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  cpn_admin_seqno = $param[sell_site]";
        $query .= "\n    AND  del_yn = 'N'";

        //조건 검색
        if ($this->blankParameterCheck($param ,"search_txt")) {
            $field = substr($param["search_cnd"], 1, -1);
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  $field LIKE '%$val%' ";
        }
        //주문 상태 검색
        if ($this->blankParameterCheck($param ,"order_state")) {
            $val = substr($param["order_state"], 1, -1);
            $query .= "\n   AND  order_state in " . $val;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 입력받은 값으로 발주번호
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    function selectCndPaper($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  paper_op_seqno ";
        $query .= "\n        ,extnl_brand_seqno ";
        $query .= "\n        ,storplace ";
        $query .= "\n FROM  paper_op ";

        //조건 검색
        if ($this->blankParameterCheck($param ,"search_txt")) {
            $field = substr($param["search_cnd"], 1, -1);
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n WHERE $field LIKE '%$val%' ";
        }

        return $conn->Execute($query);
    }

    function selectPassword($conn, $pw) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $pw = $this->parameterEscape($conn, $pw);

        $query = "select PASSWORD(%s) AS pw;";
        $query = sprintf($query, $pw);

        $rs = $conn->Execute($query);

        return $rs->fields["pw"];
    }

    /**
     * @brief 주문상태값 검색
     *
     * @param $conn = connection identifier
     *
     * @return 카테고리명
     */
    function selectStateAdmin($conn, $dvs = null) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query  = "\n   SELECT  A.state_code";
        $query .= "\n          ,A.erp_state_name";
        $query .= "\n     FROM  state_admin AS A";

        if (!empty($dvs)) {
            $dvs = $this->parameterEscape($conn, $dvs);
            $query .= "\n    WHERE  A.dvs = " . $dvs;
        }
        $query .= "\n ORDER BY  A.state_code";

        $rs = $conn->Execute($query);

        return $rs;
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

        $query  = "\n   SELECT  DISTINCT A.dvs";
        $query .= "\n     FROM  state_admin AS A";
        $query .= "\n ORDER BY  A.state_code";

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 사용 운영체제/프로그램 정보 검색
     *
     * @param $conn        = connection identifier
     * @param $param       = 검색조건 파라미터
     * @param $fields_name = 가져올 컬럼명
     *
     * @return 검색결과
     */
    function selectProTyp($conn, $param, $field_name) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        if ($field_name === "oper_sys") {
            $query  = "\n   SELECT  /* 영업.주문관리/회원 - 사용 운영체제 검색 */";
            $query .= "\n           DISTINCT oper_sys";
        } else if ($field_name === "pro") {
            $query  = "\n   SELECT  /* 영업.주문관리 - 사용 운영체제에 속한 프로그램 검색 */";
            $query .= "\n           pro";
        }
        $query .= "\n     FROM  pro_typ";
        $query .= "\n    WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "oper_sys")) {
            $query .= "\n      AND  oper_sys = " . $param["oper_sys"];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 검색회원정보애서 예외회원 정보 검색
     *
     * @detail 결재종류 - 여신한도소지금액
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectExcptMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* 영업-주문관리/회원 검색회원정보 검색용 쿼리 */";
        $query .= "\n         A.excpt_member_seqno";
        $query .= "\n        ,A.loan_limit_price";
        $query .= "\n        ,A.loan_collect_dvs";
        $query .= "\n        ,A.action_result";
        $query .= "\n        ,A.pay_promi_dvs";
        $query .= "\n        ,A.loan_pay_promi_date";
        $query .= "\n        ,A.loan_pay_promi_dvs";
        $query .= "\n   FROM  excpt_member AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 기간별 일별 집계데이터 합계 검색
     *
     * @param $conn     = connection identifier
     * @param $param    = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDaySalesStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT /* 영업-주문관리/회원 사용, 해당 회원의 금액 집계 */";
        $query .= "\n        SUM(A.sales_price) AS sum_sales_price";
        $query .= "\n       ,SUM(A.sale_price) AS sum_sale_price";
        $query .= "\n       ,SUM(A.net_sales_price) AS sum_net_price";
        $query .= "\n       ,SUM(A.depo_price) AS sum_depo_price";
        $query .= "\n FROM  day_sales_stats AS A";
        $query .= "\n WHERE  member_seqno = %s";
        $query .= "\n   AND  %s <= A.input_date";
        $query .= "\n   AND  A.input_date <= %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"])
                                , $conn->param($param["from"])
                                , $conn->param($param["to"]));

        $bind = [];
        $bind[] = $param["member_seqno"];
        $bind[] = $param["from"];
        $bind[] = $param["to"];

        $rs = $conn->Execute($query, $bind);

        return $rs->fields;
    }

    /**
     * @brief 회원 미수액 데이터 검색
     *
     * @param $conn     = connection identifier
     * @param $param    = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDayOaStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT /* 영업-주문관리/회원 사용, 미수액 정보 검색 */";
        $query .= "\n        period_end_oa";
        $query .= "\n       ,carryforward_oa";
        $query .= "\n FROM  day_sales_stats AS A";
        $query .= "\n WHERE  member_seqno = %s";
        $query .= "\n   AND  %s = A.input_date";

        $query  = sprintf($query, $conn->param($param["member_seqno"])
                                , $conn->param($param["to"]));

        $bind = [];
        $bind[] = $param["member_seqno"];
        $bind[] = $param["to"];

        $rs = $conn->Execute($query, $bind);

        if ($rs->EOF) {
            $query  = "\n SELECT /* 영업-주문관리/회원 사용, 미수액 정보 검색 */";
            $query .= "\n        period_end_oa";
            $query .= "\n       ,carryforward_oa";
            $query .= "\n  FROM  day_sales_stats";
            $query .= "\n WHERE  member_seqno = %s";
            $query .= "\n ORDER  BY input_date DESC";
            $query .= "\n LIMIT  1";

            $query  = sprintf($query, $conn->param($param["member_seqno"]));

            $bind = [];
            $bind[] = $param["member_seqno"];

            $rs = $conn->Execute($query, $bind);
        }

        return $rs->fields;
    }
}
?>
