<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/FrontCommonDAO.inc');

class OrderCommonDAO extends FrontCommonDAO {
    function __construct() {
    }

    /**
     * @brief 주문공통정보 존재하는지 확인
     *
     * @param $conn  = connection identifier
     * @param $param = 주문공통일련번호, 회원일련번호
     * @param $col   = 검색할 필드명
     *
     * @return 존재하면 true, 없으면 false
     */
    function selectOrderCommon($conn, $param, $col) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  %s";
        $query .= "\n   FROM  order_common";
        $query .= "\n  WHERE  order_common_seqno IN (%s)";
        $query .= "\n    AND  member_seqno = %s";

        $query  = sprintf($query, $col
                                , $param["order_common_seqno"]
                                , $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 제목 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function updateOrderTitle($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common";
        $query .= "\n    SET title = %s";
        $query .= "\n  WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param["title"]
                                , $param["seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 공통 상태 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function updateOrderCommonState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common";
        $query .= "\n    SET order_state = %s";
        $query .= "\n  WHERE order_common_seqno = %s";
        $query .= "\n    AND member_seqno       = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["order_common_seqno"]
                                , $param["member_seqno"]);

        return $conn->Execute($query);
    }

    function insertOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO order_info_history ";
        $query .= "\n       (order_common_seqno, kind, before_detail, after_detail, empl_id) VALUES (%s, %s, %s, %s, %s)";

        $query  = sprintf($query, $param["order_common_seqno"], $param["kind"], $param["before"], $param["after"], $param["empl_id"]);
        return $conn->Execute($query);
    }

    public function insertStateHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param['date'] = "NOW()";
        if($param['state'] == '2680') {
            $param['date'] = "DATE_ADD(NOW(), INTERVAL 5 SECOND)";
        }

        $query = " INSERT INTO order_state_history (order_common_seqno, state, state_modi_date, empl_id, detail) ";
        $query .= " SELECT order_common_seqno, '%s', %s, '%s', '%s' FROM order_common ";
        $query .= " WHERE order_num = '%s' ";

        $query = sprintf($query, $param['state'], $param['date'], $param['empl_id'],$param['detail'],$param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    /**
     * @brief 주문 상세 상태 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function updateOrderDetailState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_detail";
        $query .= "\n    SET state = %s";
        $query .= "\n  WHERE order_common_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["order_common_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문 상세 건수 파일 상태 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function updateOrderDetailCountFileState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_detail_count_file";
        $query .= "\n    SET state = %s";
        $query .= "\n  WHERE order_detail_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["order_detail_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문상세 일련번호 검색
     *
     * @detail 인쇄유형, 페이지, 종이
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 쿼리실행결과
     */
    function selectOrderDetailSeqno($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $seqno = $this->parameterEscape($conn, $seqno);

        $query  = "\n SELECT  A.order_detail_seqno";
        $query .= "\n   FROM  order_detail AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $seqno);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문공통 테이블 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 데이터 파라미터
     *
     * @return option html
     */
    function selectCartOrderList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $dvs = $param["dvs"];

        $except_arr = ["order_common_seqno" => true,
                       "s_num" => true,
                       "list_num" => true];
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT ";
        if ($dvs == "COUNT") {
            $query .= "\n         COUNT(*) as cnt";
        } else {
            $query .= "\n         A.order_regi_date";
            $query .= "\n        ,A.order_common_seqno";
            $query .= "\n        ,A.order_num";
            $query .= "\n        ,A.title";
            $query .= "\n        ,A.order_detail";
            $query .= "\n        ,A.sell_price";
            $query .= "\n        ,A.grade_sale_price";
            $query .= "\n        ,A.member_sale_price";
            $query .= "\n        ,A.add_after_price";
            $query .= "\n        ,A.add_opt_price";
            $query .= "\n        ,A.event_price";
            $query .= "\n        ,A.expec_weight";
            $query .= "\n        ,A.cate_sortcode";
            $query .= "\n        ,A.dlvr_dvs";
            $query .= "\n        ,IF(A.amt < 1, A.amt, TRIM(TRAILING '.0' FROM CAST(A.amt AS char))) AS amt";
            $query .= "\n        ,A.amt_unit_dvs";
            $query .= "\n        ,A.expec_release_date";
            $query .= "\n        ,A.count";
            $query .= "\n        ,B.order_file_seqno";
            $query .= "\n        ,B.origin_file_name";
            $query .= "\n        ,C.stan_name";
            $query .= "\n        ,C.cate_paper_mpcode";
        }

        $query .= "\n   FROM  order_common AS A";
        //$query .= "\n        ,order_file   AS B";
        $query .= "\n LEFT OUTER JOIN order_file AS B";
        $query .= "\n   ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n   INNER JOIN order_detail AS C   ON A.order_common_seqno = C.order_common_seqno ";

        $query .= "\n  WHERE  A.member_seqno = %s";
        //$query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        //$query .= "\n    AND  A.member_seqno = %s";
        if ($this->blankParameterCheck($param, "order_state")) {
            $query .= "\n    AND  A.order_state  = %s ";
        }
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $query .= "\n    AND  A.order_common_seqno IN (";
            $query .= $param["order_common_seqno"];
            $query .= ')';
        }
        $query .= "\n ORDER BY A.order_regi_date DESC";
        $query  = sprintf($query, $param["member_seqno"], $param["order_state"]);
        if (!empty($param["s_num"]) && !empty($param["list_num"])) {
            $query .= "\n  LIMIT " . $param["s_num"] . ", " . $param["list_num"];
        }

        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 주문에 해당하는 후공정 내역 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 주문_상세_구분_번호
     *
     * @return 검색결과
     */
    function selectOrderAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.after_name AS name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.basic_yn";
        $query .= "\n        ,A.order_after_history_seqno";
        $query .= "\n        ,A.mpcode";
        $query .= "\n        ,A.seq";
        $query .= "\n        ,A.detail";
        $query .= "\n   FROM  order_after_history AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $query .= "\n    AND  A.order_common_seqno = ";
            $query .= $param["order_common_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_detail_dvs_num")) {
            $query .= "\n    AND  A.order_detail_dvs_num = ";
            $query .= $param["order_detail_dvs_num"];
        }
        if ($this->blankParameterCheck($param, "basic_yn")) {
            $query .= "\n    AND  A.basic_yn = ";
            $query .= $param["basic_yn"];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 추가정보 생성에 필요한 정보 검색
     *
     * @detail 후공정 정보
     *
     * @param $conn  = connection identifier
     * @param $param = 주문 공통 일련번호
     *
     * @return 쿼리실행결과
     */
    function selectOrderOptHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.opt_name AS name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.basic_yn";
        $query .= "\n        ,A.mpcode";
        $query .= "\n        ,A.detail";
        $query .= "\n   FROM  order_opt_history AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_common_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고리에 해당하는 첫 번째 사진 검색
     *
     * @param $conn  = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function selectCatePhoto($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"] = "CONCAT(file_path, save_file_name) AS full_path";
        $temp["table"] = "cate_photo";
        $temp["where"]["cate_sortcode"] = $cate_sortcode;
        $temp["where"]["seq"] = '1';

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["full_path"];
    }

    /**
     * @brief 주문 데이터 검색
     *
     * @param $conn  = connection identifer
     * @param $param = 주문_공통_일련번호, 회원_일련번호
     *
     * @return 쿼리실행결과
     */
    function selectOrderInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [OrderCommonDAO.inc] 주문 데이터 검색 */";
        $query .= "\n         A.title";
        $query .= "\n        ,A.opt_use_yn";
        $query .= "\n        ,A.file_upload_dvs";
        $query .= "\n        ,A.page_cnt";
        $query .= "\n        ,B.order_detail_seqno   AS s_detail_seqno";
        $query .= "\n        ,B.order_detail_dvs_num AS s_detail_dvs_num";
        $query .= "\n        ,B.cate_sortcode        AS s_cate_sortcode";
        $query .= "\n        ,B.amt                  AS s_amt";
        $query .= "\n        ,B.amt_unit_dvs         AS s_amt_unit_dvs";
        $query .= "\n        ,B.sell_price           AS s_sell_price";
        $query .= "\n        ,B.grade_sale_price     AS s_grade_sale_price";
        $query .= "\n        ,B.add_after_price      AS s_after_price";
        $query .= "\n        ,B.after_use_yn         AS s_after_use_yn";
        $query .= "\n        ,B.stan_name            AS s_stan_name";
        $query .= "\n        ,C.order_detail_brochure_seqno AS b_detail_seqno";
        $query .= "\n        ,C.order_detail_dvs_num        AS b_detail_dvs_num";
        $query .= "\n        ,C.cate_sortcode               AS b_cate_sortcode";
        $query .= "\n        ,C.page_amt                    AS b_page_amt";
        $query .= "\n        ,C.amt                         AS b_amt";
        $query .= "\n        ,C.amt_unit_dvs                AS b_amt_unit_dvs";
        $query .= "\n        ,C.sell_price                  AS b_sell_price";
        $query .= "\n        ,C.grade_sale_price            AS b_grade_sale_price";
        $query .= "\n        ,C.add_after_price             AS b_after_price";
        $query .= "\n        ,C.after_use_yn                AS b_after_use_yn";
        $query .= "\n   FROM order_common AS A";
        $query .= "\n   LEFT OUTER JOIN order_detail AS B";
        $query .= "\n     ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n   LEFT OUTER JOIN order_detail_brochure AS C";
        $query .= "\n     ON A.order_common_seqno = C.order_common_seqno";
        $query .= "\n  WHERE A.order_common_seqno = %s";
        $query .= "\n    AND A.member_seqno       = %s";

        $query  = sprintf($query, $param["order_common_seqno"]
                                , $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    function selectDirectDlvrInfo($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  dlvr_add_info ";
        $query .= "\n   FROM member ";
        $query .= "\n   WHERE member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields["dlvr_add_info"];
    }

    /**
     * @brief 주문_파일 정보 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $flag = false;

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.file_path";
        $query .= "\n        ,A.save_file_name";
        $query .= "\n        ,A.origin_file_name";
        $query .= "\n        ,A.order_file_seqno";
        $query .= "\n        ,A.size";
        $query .= "\n   FROM  order_file AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "order_file_seqno")) {
            $query .= "\n    AND  A.order_file_seqno   = ";
            $query .= $param["order_file_seqno"];
            $flag = true;
        }
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno       = ";
            $query .= $param["member_seqno"];
            $flag = true;
        }
        if ($this->blankParameterCheck($param, "order_common_seqno")) {

            $query .= "\n    AND  A.order_common_seqno = ";
            $query .= $param["order_common_seqno"];
            $flag = true;
        }

        if (!$flag) {
            return [];
        }

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 주문_파일 정보 삭제
     *
     * @param $conn = connection identifier
     * @param $param = 삭제조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function deleteOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n DELETE";
        $query .= "\n   FROM order_file";
        $query .= "\n  WHERE member_seqno = %s";
        if ($this->blankParameterCheck($param, "order_file_seqno")) {
            $query .= "\n    AND order_file_seqno   = ";
            $query .= $param["order_file_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $query .= "\n    AND order_common_seqno = ";
            $query .= $param["order_common_seqno"];
        }

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    function updateToAuto($conn, $order_common_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query  = "\n UPDATE order_common";
        $query .= "\n   set receipt_dvs = 'Auto' ";
        $query .= "\n  WHERE order_common_seqno = " . $order_common_seqno;

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 회원이 신청했던 직배정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDirectDlvrReq($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  /* OrderCommonDAO.inc - 회원 직배정보 검색 */";
        $query .= "\n         A.zipcode";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";
        $query .= "\n        ,A.tel_num";
        $query .= "\n        ,A.cell_num";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        return $conn->Execute($query);
    }

    /**
     * @brief 원파일 주문 그룹값 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOnefileOrderGroup($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO onefile_order_group (";
        $query .= "\n      order_common_seqno";
        $query .= "\n     ,group_num";
        $query .= "\n     ,order_num";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,%s";
        $query .= "\n )";
        $query .= "\n ON DUPLICATE KEY UPDATE group_num = %s";

        $query  = sprintf($query, $param["order_common_seqno"]
                                , $param["group_num"]
                                , $param["order_num"]
                                , $param["group_num"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 원파일 주문 그룹정보 삭제
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function deleteOnefileOrderGroup($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $flag = false;

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n DELETE";
        $query .= "\n   FROM onefile_order_group";
        $query .= "\n  WHERE 1 = 1";
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $flag = true;
            $query .= "\n    AND order_common_seqno = ";
            $query .= $param["order_common_seqno"];
        }
        if ($this->blankParameterCheck($param, "order_num")) {
            $flag = true;
            $query .= "\n    AND order_num = ";
            $query .= $param["order_num"];
        }
        if ($this->blankParameterCheck($param, "group_num")) {
            $flag = true;
            $query .= "\n    AND group_num= ";
            $query .= $param["group_num"];
        }

        if (!$flag) {
            return false;
        }

        return $conn->Execute($query);
    }


}
