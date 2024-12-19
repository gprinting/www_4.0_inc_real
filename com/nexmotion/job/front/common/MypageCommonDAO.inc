<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/FrontCommonDAO.inc');

class MypageCommonDAO extends FrontCommonDAO {

    function __construct() {
    }

    /**
     * @brief 개인 기업 일련번호 가져옴
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function getBuPerSeqno($conn, $param) {
     
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  member_seqno ";
        $query .= "\n   FROM  member ";
        $query .= "\n  WHERE  group_id =" . $param["member_seqno"];
        //$query .= "\n    AND  withdraw_dvs = 1";

        return $conn->Execute($query);
    }

    /**
     * @brief 주문에 해당하는 후공정 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  after_name";
        $query .= "\n        ,depth1";
        $query .= "\n        ,depth2";
        $query .= "\n        ,depth3";
        $query .= "\n   FROM  order_after_history";
        $query .= "\n  WHERE  order_common_seqno = ". $param["order_seqno"];
        if ($this->blankParameterCheck($param ,"basic_yn")) {
            $query .= "\n    AND  basic_yn = ". $param["basic_yn"];
        }

        $result = $conn->Execute($query);

        return $result;

    }

    //주문 수
    function selectOrderStatusCount($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, ["not" => true]);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        $query .= "\nSELECT  COUNT(*) AS cnt ";
        $query .= "\n  FROM  order_common AS A ";
        $query .= "\n WHERE  member_seqno IN(" . $member_seqno . ") AND A.order_state != '1080' AND A.OPI_Date IS NULL ";
 
        //제외 
        if ($this->blankParameterCheck($param ,"not")) {
            $query .= "\n   AND  A.order_state NOT IN (";
            $query .= $param["not"];
            $query .= ')';
        }

        //상태
        if ($this->blankParameterCheck($param ,"state_min")) {
            $query .= "\n      AND (" . $param["state_min"] . " + 0) <= A.order_state";
            $query .= "\n      AND A.order_state <= (" . $param["state_max"] . " + 0)";
        }
 
        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .="\n       AND A.depo_finish_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {

            $to = substr($param["to"], 1, -1);

            $query .="\n       AND A.depo_finish_date <= '" . $to;
            $query .=" 23:59:59'";
        }
        return $conn->Execute($query);
    }

    //마이페이지 기업 메인 주문리스트 쿼리
    function selectMainBusiOrderList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        if ($dvs === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n SELECT A.order_regi_date";
            $query .= "\n       ,A.order_num";
            $query .= "\n       ,A.member_seqno";
            $query .= "\n       ,A.title";
            $query .= "\n       ,A.order_detail";
            $query .= "\n       ,A.amt";
            $query .= "\n       ,A.amt_unit_dvs";
            $query .= "\n       ,A.count";
            $query .= "\n       ,A.pay_price";
            $query .= "\n       ,A.order_state";
            $query .= "\n       ,A.order_common_seqno";
            $query .= "\n       ,B.member_name";
        }
        $query .= "\n  FROM  order_common AS A ";
        $query .= "\n       ,member AS B ";
        $query .= "\n WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n   AND  A.member_seqno IN(" . $member_seqno . ")";
 
        //제외 
        if ($this->blankParameterCheck($param ,"not")) {
            $query .= "\n   AND  A.order_state != " . $param["not"];
        }
 
        //메인 상태 검색
        if ($this->blankParameterCheck($param ,"state_min")) {
            $query .= "\n      AND (" . $param["state_min"] . " + 0) <= A.order_state";
            $query .= "\n      AND A.order_state <= (" . $param["state_max"] . " + 0)";
        }

        //상태
        if ($this->blankParameterCheck($param ,"order_state")) {
            $query .= "\n   AND  A.order_state = " . $param["order_state"];
        }
 
        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .="\n       AND A.order_regi_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {

            $to = substr($param["to"], 1, -1);

            $query .="\n       AND A.order_regi_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $query .= "\n ORDER BY A.order_common_seqno DESC ";

        return $conn->Execute($query);
    }
 
    //마이페이지 기업을 제외한 메인 주문리스트 쿼리
    function selectMainOrderList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        if ($dvs === "COUNT") {
            $query  = "\n SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n SELECT A.order_regi_date";
            $query .= "\n       ,A.order_num";
            $query .= "\n       ,A.member_seqno";
            $query .= "\n       ,A.title";
            $query .= "\n       ,A.order_detail";
            $query .= "\n       ,A.amt";
            $query .= "\n       ,A.amt_unit_dvs";
            $query .= "\n       ,A.count";
            $query .= "\n       ,A.pay_price";
            $query .= "\n       ,A.order_state";
            $query .= "\n       ,A.order_common_seqno";
        }
        $query .= "\n  FROM  order_common AS A ";
        $query .= "\n WHERE  A.member_seqno IN(" . $member_seqno . ")";
 
        //제외 
        if ($this->blankParameterCheck($param ,"not")) {
            $query .= "\n   AND  A.order_state != " . $param["not"];
        }

        //상태
        if ($this->blankParameterCheck($param ,"order_state")) {
            $query .= "\n   AND  A.order_state = " . $param["order_state"];
        }
  
        //메인 상태 검색
        if ($this->blankParameterCheck($param ,"state_min")) {
            $query .= "\n      AND (" . $param["state_min"] . " + 0) <= A.order_state";
            $query .= "\n      AND A.order_state <= (" . $param["state_max"] . " + 0)";
        }

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .="\n       AND A.order_regi_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {

            $to = substr($param["to"], 1, -1);

            $query .="\n       AND A.order_regi_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $query .= "\n ORDER BY A.order_common_seqno DESC ";

        return $conn->Execute($query);
    }

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
        $query .= "\n        ,A.email";

        $query .= "\n   FROM  licensee_info AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 기업을 주문 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectBusiOrderList($conn, $param) {

        if ($this->connectionCheck($conn) === false) { return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        $query  = "\n   SELECT /* MypageCommonDAO.inc - selectBusiOrderList */";
        $query .= "\n          A.order_regi_date";
        $query .= "\n         ,A.order_num";
        $query .= "\n         ,A.member_seqno";
        $query .= "\n         ,A.title";
        $query .= "\n         ,A.order_detail";
        $query .= "\n         ,A.amt";
        $query .= "\n         ,A.amt_unit_dvs";
        $query .= "\n         ,A.count";
        $query .= "\n         ,A.pay_price";
        $query .= "\n         ,A.order_state";
        $query .= "\n         ,A.order_common_seqno";
        $query .= "\n         ,C.member_name";
        $query .= "\n         ,SUM(B.dlvr_price) AS dlvr_price";

        $query .= "\n     FROM order_common AS A";
        $query .= "\nLEFT JOIN order_dlvr AS B";
        $query .= "\n       ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\nLEFT JOIN member AS C";
        $query .= "\n       ON A.member_seqno = C.member_seqno";
        $query .= "\n    WHERE A.member_seqno IN(" . $member_seqno . ")";
        $query .= "\n      AND B.tsrs_dvs = '수신' AND A.OPI_Date IS NULL ";

        //인쇄물제목 검색
        if ($this->blankParameterCheck($param ,"title")) {
            $title = substr($param["title"], 1, -1);
            $query .= "\n      AND A.title LIKE '%" . $title . "%'";
        }

        //배송종류
        if ($this->blankParameterCheck($param ,"dlvr_way")) {
            $search_txt = substr($param["dlvr_way"], 1, -1);
            $query .= "\n      AND B.dlvr_way = " . $param["dlvr_way"];
        }

        //상태
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n      AND A.order_state = " . $param["state"];
        }
        if ($this->blankParameterCheck($param ,"state_min")) {
            $query .= "\n      AND (" . $param["state_min"] . " + 0) <= A.order_state";
            $query .= "\n      AND A.order_state <= (" . $param["state_max"] . " + 0)";
        }

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .="\n       AND A.order_regi_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {

            $to = substr($param["to"], 1, -1);

            $query .="\n       AND A.order_regi_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs != "COUNT") {
            $query .= "\n GROUP BY A.order_common_seqno";
            $query .= "\n ORDER BY A.order_common_seqno DESC ";
            $query .= "\n LIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 기업을 제외한 주문 리스트
     * @modified 180202 이청산 : 배송묶기 관련 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        $query  = "\n   SELECT ";
        $query .= "\n          SQL_CALC_FOUND_ROWS";
        $query .= "\n          A.depo_finish_date";
        $query .= "\n         ,A.order_num";
        $query .= "\n         ,A.member_seqno";
        $query .= "\n         ,A.title";
        $query .= "\n         ,A.order_detail";
        $query .= "\n         ,A.order_regi_date";        
        $query .= "\n         ,A.amt";
        $query .= "\n         ,A.amt_unit_dvs";
        $query .= "\n         ,A.count";
        $query .= "\n         ,A.pay_price";
        $query .= "\n         ,A.order_state";
        $query .= "\n         ,A.order_common_seqno";
        $query .= "\n         ,A.file_upload_dvs";
        $query .= "\n         ,A.order_mng";
        $query .= "\n         ,C.member_name";
        $query .= "\n         ,SUM(B.dlvr_price) AS dlvr_price";
        $query .= "\n         ,B.bun_dlvr_order_num";
        $query .= "\n         ,B.dlvr_way";
        $query .= "\n         ,B.bun_group";
        $query .= "\n         ,D.rework_cause";
        $query .= "\n         ,F.front_state_name";
        $query .= "\n         ,G.deal_num";
        $query .= "\n         ,COUNT(B.bun_dlvr_order_num) AS bun_dlvr ";
        $query .= "\n         ,CONCAT(' / ',H.cut_size_wid,'*',H.cut_size_vert) as size";
        $query .= "\n    FROM  order_common AS A";
        $query .= "\n    INNER JOIN order_dlvr   AS B ON A.order_common_seqno = B.order_common_seqno AND B.tsrs_dvs = '수신' ";
        $query .= "\n    INNER JOIN member       AS C ON A.member_seqno = C.member_seqno";
        $query .= "\n    INNER JOIN state_admin       AS F ON  A.order_state = F.state_code";
        $query .= "\n    LEFT JOIN member_pay_history       AS G ON  A.order_num = G.order_num AND G.dvs = '매출증가'";
        $query .= "\n    LEFT JOIN rework_list AS D ON A.order_common_seqno = D.rework_order_common_seqno";
        $query .= "\n    LEFT JOIN order_detail AS H ON A.order_common_seqno = H.order_common_seqno";
        //$query .= "\n    INNER JOIN order_dlvr AS E ON B.bun_dlvr_order_num = E.bun_dlvr_order_num AND E.tsrs_dvs = '수신'";
        $query .= "\n   WHERE  A.member_seqno IN (" . $member_seqno . ")";
        $query .= "\n     AND  A.order_state NOT IN ('1080') AND A.OPI_Date IS NULL ";
        if ($this->blankParameterCheck($param ,"order_common_seqno")) {
            $query .= "\n     AND  A.order_common_seqno = ";
            $query .= $param["order_common_seqno"];
        } else {
            //인쇄물제목 검색
            if ($this->blankParameterCheck($param ,"title")) {
                $title = substr($param["title"], 1, -1);
                $query .= "\n     AND  A.title LIKE '%" . $title . "%'";
            }
            //배송종류
            if ($this->blankParameterCheck($param ,"dlvr_way")) {
                $search_txt = substr($param["dlvr_way"], 1, -1);
                $query .= "\n     AND  B.dlvr_way = " . $param["dlvr_way"];
            }
            // 주문상태
            if ($this->blankParameterCheck($param ,"state")) {
                $query .= "\n     AND  A.order_state = " . $param["state"];
            }
            if ($this->blankParameterCheck($param ,"state_min")) {
                $query .= "\n     AND  (" . $param["state_min"] . " + 0) <= A.order_state";
                $query .= "\n     AND  A.order_state <= (" . $param["state_max"] . " + 0)";
            }
            //등록일
            if ($this->blankParameterCheck($param ,"from")) {
                $from = substr($param["from"], 1, -1);
                $query .="\n      AND  A.depo_finish_date >= '" . $from;
                $query .=" 00:00:00'";
            }
            if ($this->blankParameterCheck($param ,"to")) {
                $to = substr($param["to"], 1, -1);

                $query .="\n      AND  A.depo_finish_date <= '" . $to;
                $query .=" 23:59:59'";
            }
            $query .= "\n    GROUP BY  A.order_common_seqno";
            $query .= "\n    ORDER BY  A.depo_finish_date DESC, A.order_common_seqno DESC ";
            //$query .= "\n ORDER BY  T1.order_regi_date    DESC";
            //$query .= "\n          ,T1.bun_dlvr_order_num DESC";
            //$query .= "\n          ,T1.order_common_seqno DESC";
            $query .= "\n    LIMIT " . $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 주문공통 테이블 검색, 주문상세정보중 공통정보 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommon($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT C.file_path";
        $query .= "\n         ,C.save_file_name";
        $query .= "\n         ,A.title";
        $query .= "\n         ,A.amt";
        $query .= "\n         ,A.count";
        $query .= "\n         ,A.amt_unit_dvs";
        $query .= "\n         ,A.event_price";
        $query .= "\n         ,A.use_point_price";
        $query .= "\n         ,A.sell_price";
        $query .= "\n         ,A.pay_price";
        $query .= "\n         ,A.grade_sale_price";
        $query .= "\n         ,A.member_sale_price";
        $query .= "\n         ,A.add_after_price";
        $query .= "\n         ,A.add_opt_price";
        $query .= "\n         ,A.order_state";
        $query .= "\n         ,A.order_detail";
        $query .= "\n         ,A.expec_weight";
        $query .= "\n         ,D.dlvr_way";
        $query .= "\n         ,D.zipcode";
        $query .= "\n         ,D.addr";
        $query .= "\n         ,D.addr_detail";
        $query .= "\n         ,SUM(D.dlvr_price) AS dlvr_price";
        $query .= "\n     FROM order_common AS A";
        $query .= "\nLEFT JOIN cate_photo AS C";
        $query .= "\n       ON A.cate_sortcode = C.cate_sortcode";
        $query .= "\n      AND C.seq = '1'";
        $query .= "\nLEFT JOIN order_dlvr AS D ";
        $query .= "\n       ON A.order_common_seqno = D.order_common_seqno";
        $query .= "\n      AND D.tsrs_dvs = '수신'";
        $query .= "\n    WHERE A.order_common_seqno = " . $param["order_common_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 주문공통 테이블 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderCommonBasicInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT A.title";
        $query .= "\n         ,A.order_num";
        $query .= "\n         ,A.order_regi_date";
        $query .= "\n         ,D.dlvr_way";
        $query .= "\n     FROM order_common AS A";
        $query .= "\nLEFT JOIN order_dlvr AS D";
        $query .= "\n       ON A.order_common_seqno = D.order_common_seqno";
        $query .= "\n      AND D.tsrs_dvs ='수신'";
        $query .= "\n    WHERE A.order_common_seqno = " . $param["order_common_seqno"];

        return $conn->Execute($query);
    }


    /**
     * @brief 주문상세 테이블 검색, 주문상세정보 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetail($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("table" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n   SELECT A.order_detail";
        $query .= "\n         ,A.state";
        $query .= "\n         ,A.grade_sale_price";
        $query .= "\n         ,A.amt";
        $query .= "\n         ,A.amt_unit_dvs";
        $query .= "\n         ,A.pay_price";
        $query .= "\n         ,A.%s_seqno AS seqno";
        if ($param["table"] === "order_detail") {
            $query .= "\n         ,A.count";
            $query .= "\n         ,A.expec_weight";
        }
        $query .= "\n         ,A.order_detail_dvs_num";
        $query .= "\n     FROM %s AS A";
        $query .= "\n    WHERE A.order_common_seqno = %s";

        $query  = sprintf($query, $param["table"]
                                , $param["table"]
                                , $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 직배신청기간 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDirectDlvrReqPeriod($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  /* MemberDlvrDAO.inc - 회원 직배기간 검색 */";
        $query .= "\n         A.start_period";
        $query .= "\n        ,A.end_period";
        $query .= "\n   FROM  direct_dlvr_req AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 배송비 업데이트
     *
     * @param $conn  = connection identifier
     * @param $param = 배송비, 주문배송일련번호
     *
     * @return 검색결과
     */
    public function updateOrderDlvrPrice($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n UPDATE  /* [MypageCommonDAO.inc] - updateOrderDlvrPrice */";
        $query .= "\n         order_dlvr";
        $query .= "\n    SET  dlvr_price = %s";
        $query .= "\n  WHERE  order_dlvr_seqno = %s";

        $query  = sprintf($query, $conn->param($param["dlvr_price"])
                                , $conn->param($param["order_dlvr_seqno"]));

        $temp = [
             $param["dlvr_price"]
            ,$param["order_dlvr_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 주문별 배송비 이전, 그룹여부 변경
     *
     * @param $conn  = connection identifier
     * @param $param = 배송비, 주문배송일련번호
     *
     * @return 검색결과
     */
    public function updateOrderCommon($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n UPDATE  /* [MypageCommonDAO.inc] - updateOrderCommon */";
        $query .= "\n         order_common";
        $query .= "\n    SET  depo_price = depo_price + %s";
        $query .= "\n        ,pay_price  = pay_price + %s";
        if ($this->blankParameterCheck($param, "bun_yn")) {
            $query .= "\n        ,bun_yn     = ";
            $query .= $this->parameterEscape($conn, $param["bun_yn"]);
        }
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["depo_price"])
                                , $conn->param($param["pay_price"])
                                , $conn->param($param["order_common_seqno"]));

        $temp = [
             $param["depo_price"] ?? 0
            ,$param["pay_price"] ?? 0
            ,$param["order_common_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 주문 상세 팝업 정보 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT A.order_num";
        $query .= "\n         ,A.order_detail";
        $query .= "\n         ,A.title";
        $query .= "\n         ,A.use_point_price";
        $query .= "\n         ,A.count";
        $query .= "\n         ,A.order_regi_date";
        $query .= "\n         ,A.order_state";
        $query .= "\n         ,A.amt";
        $query .= "\n         ,A.amt_unit_dvs";
        $query .= "\n         ,A.work_memo";
        $query .= "\n         ,A.cust_memo";
        $query .= "\n         ,F.name as order_mng";
        $query .= "\n         ,B.file_path";
        $query .= "\n         ,B.save_file_name";
        $query .= "\n         ,B.origin_file_name";
        $query .= "\n         ,B.size";
        $query .= "\n         ,B.preview_file_path";
        $query .= "\n         ,B.preview_file_name";
        $query .= "\n         ,C.member_name";
        $query .= "\n         ,C.tel_num";
        $query .= "\n         ,C.cell_num";
        $query .= "\n         ,C.mail";
        $query .= "\n         ,C.zipcode";
        $query .= "\n         ,C.addr";
        $query .= "\n         ,C.addr_detail";
        $query .= "\n         ,D.paper_sum_price";
        $query .= "\n         ,D.output_sum_price";
        $query .= "\n         ,D.print_sum_price";
        $query .= "\n         ,D.sell_price";
        $query .= "\n         ,D.add_after_price";
        $query .= "\n         ,D.mono_yn";
        $query .= "\n     FROM order_common AS A";
        $query .= "\n     INNER JOIN order_file AS B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n     INNER JOIN member AS C ON A.member_seqno = C.member_seqno";
        $query .= "\n     INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= "\n     LEFT JOIN empl AS E ON A.order_mng = E.empl_id ";
        $query .= "\n     LEFT JOIN empl_human_info AS F ON E.empl_seqno = F.empl_seqno ";
        $query .= "\n    WHERE 1 = 1";
        $query .= "\n      AND A.order_common_seqno = " . $param["order_common_seqno"];

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 상세 팝업 상태 정보 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailStateInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT A.state";
        $query .= "\n         ,A.state_modi_date";
        $query .= "\n         ,B.front_state_name";
        $query .= "\n     FROM order_state_history AS A";
        $query .= "\n         ,state_admin AS B";
        $query .= "\n    WHERE A.state = B.state_code";
        $query .= "\n      AND A.order_common_seqno = " . $param["order_common_seqno"];
        $query .= "\n      AND B.front_state_name LIKE '%완료' ";
        $query .= "\n GROUP BY  B.front_state_name ";
        $query .= "\n ORDER BY  A.state_modi_date ASC";

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 상세 팝업 배송 정보 검색
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailDlvrInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n   SELECT *";
        $query .= "\n     FROM order_dlvr";
        $query .= "\n    WHERE order_common_seqno = " . $param["order_common_seqno"];
        $query .= "\n      AND tsrs_dvs = " . $param["tsrs_dvs"];

        return $conn->Execute($query);
    }
}
?>
