<?
include_once(INC_PATH . "/com/nexmotion/job/front/common/MypageCommonDAO.inc");
include_once(INC_PATH . '/com/nexmotion/html/front/mypage/OrderInfoHTML.inc');

/**
 * @file OtoInqMngDAO.php
 *
 * @brief 마이페이지 - 메인
 */
class MainDAO extends MypageCommonDAO {

    function __construct() {
    }

    /**
     * @brief 1:1문의 요약리스트
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOtoInquireSummary($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        $query  = "\nSELECT  T1.* ";
        $query .= "\n  FROM  ( ";
        $query .= "\n           SELECT  A.regi_date AS inq_date ";
        $query .= "\n                  ,A.member_seqno ";
        $query .= "\n                  ,A.inq_typ ";
        $query .= "\n                  ,A.title ";
        $query .= "\n                  ,B.regi_date AS reply_date ";
        $query .= "\n                  ,C.name ";
        $query .= "\n                  ,A.answ_yn ";
        $query .= "\n                  ,A.oto_inq_seqno ";
        $query .= "\n            FROM  oto_inq AS A ";
        $query .= "\n       LEFT JOIN  oto_inq_reply AS B ";
        $query .= "\n              ON  A.oto_inq_seqno = B.oto_inq_seqno ";
        $query .= "\n       LEFT JOIN  empl_human_info AS C ";
        $query .= "\n              ON  B.empl_seqno = C.empl_seqno ) AS T1 ";
        $query .= "\n         ,member AS T2 ";
        $query .= "\n WHERE  T1.member_seqno = T2.member_seqno ";
        $query .="\n    AND  T2.member_seqno IN(" . $member_seqno . ")";
        $query .= "\n ORDER BY T1.oto_inq_seqno DESC ";
        $query .= "\n LIMIT 0, 5";

        return $conn->Execute($query);
    }

    function selectOrderList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $member_seqno = substr($param["member_seqno"], 1, -1);


        $query  = "\n   SELECT ";
        $query .= "\n          SQL_CALC_FOUND_ROWS";
        $query .= "\n          A.order_regi_date";
        $query .= "\n         ,A.depo_finish_date";
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
        $query .= "\n         ,B.bun_dlvr_order_num";
        $query .= "\n         ,B.dlvr_way";
        $query .= "\n         ,B.bun_group";
        $query .= "\n         ,D.rework_cause";
        $query .= "\n         ,F.front_state_name";
        $query .= "\n    FROM  order_common AS A LEFT JOIN rework_list AS D ON A.order_common_seqno = D.rework_order_common_seqno";
        $query .= "\n         ,order_dlvr   AS B";
        $query .= "\n         ,member       AS C";
        $query .= "\n         ,state_admin       AS F";
        $query .= "\n   WHERE  A.member_seqno IN (" . $member_seqno . ")";
        $query .= "\n     AND  A.OPI_Date IS NULL ";
        $query .= "\n     AND  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n     AND  A.member_seqno = C.member_seqno";
        $query .= "\n     AND  A.order_state = F.state_code";
        $query .= "\n     AND  A.depo_finish_date IS NOT NULL ";
        $query .= "\n     AND  A.OPI_Date IS NULL ";

        //if ($this->blankParameterCheck($param ,"from")) {
        //    $from = substr($param["from"], 1, -1);
        //    $query .="\n      AND  A.depo_finish_date >= '" . $from;
        //    $query .=" 00:00:00'";
        //}
        //if ($this->blankParameterCheck($param ,"to")) {
        //    $to = substr($param["to"], 1, -1);
        //    $query .="\n      AND  A.depo_finish_date <= '" . $to;
        //    $query .=" 23:59:59'";
        //}
        $query .= "\n    GROUP BY  A.order_common_seqno";
        $query .= "\n    ORDER BY  A.depo_finish_date DESC";
        //$query .= "\n ORDER BY  T1.order_regi_date    DESC";
        //$query .= "\n          ,T1.bun_dlvr_order_num DESC";
        //$query .= "\n          ,T1.order_common_seqno DESC";
        $query .= "\n    LIMIT 5";

        return $conn->Execute($query);
    }

    /**
     * @brief 배송지 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDlvrList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $dvs  = substr($param["dvs"], 1, -1);

        $query  = "\n    SELECT  /* [MemberDlvrDAO.inc] 주소록 검색 */";
        $query .= "\n            SQL_CALC_FOUND_ROWS ";
        $query .= "\n            member_dlvr_seqno";
        $query .= "\n           ,dlvr_name";
        $query .= "\n           ,regi_date";
        $query .= "\n           ,recei";
        $query .= "\n           ,tel_num";
        $query .= "\n           ,cell_num";
        $query .= "\n           ,ifnull(cell_num, tel_num) tel";
        $query .= "\n           ,zipcode";
        $query .= "\n           ,addr";
        $query .= "\n           ,addr_detail";
        $query .= "\n           ,member_seqno";
        $query .= "\n           ,basic_yn";
        $query .= "\n      FROM  member_dlvr";
        $query .= "\n     WHERE  member_seqno = " . $param["member_seqno"];


        $query .= "\n ORDER BY member_dlvr_seqno DESC ";

        return $conn->Execute($query);
    }

    function selectCartList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $member_seqno = substr($param["member_seqno"], 1, -1);


        $query  = "\n   SELECT ";
        $query .= "\n          SQL_CALC_FOUND_ROWS";
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
        $query .= "\n         ,D.rework_cause";
        $query .= "\n         ,F.front_state_name";
        $query .= "\n    FROM  order_common AS A LEFT JOIN rework_list AS D ON A.order_common_seqno = D.rework_order_common_seqno";
        $query .= "\n         ,member       AS C";
        $query .= "\n         ,state_admin       AS F";
        $query .= "\n   WHERE  A.member_seqno IN (" . $member_seqno . ")";
        $query .= "\n     AND  A.member_seqno = C.member_seqno";
        $query .= "\n     AND  A.order_state = F.state_code";
        $query .= "\n     AND  A.order_state = '1120' ";

        //if ($this->blankParameterCheck($param ,"from")) {
        //    $from = substr($param["from"], 1, -1);
        //    $query .="\n      AND  A.depo_finish_date >= '" . $from;
        //    $query .=" 00:00:00'";
        //}
        //if ($this->blankParameterCheck($param ,"to")) {
        //    $to = substr($param["to"], 1, -1);
        //    $query .="\n      AND  A.depo_finish_date <= '" . $to;
        //    $query .=" 23:59:59'";
        //}
        $query .= "\n    GROUP BY  A.order_common_seqno";
        $query .= "\n    ORDER BY  A.order_common_seqno DESC";
        //$query .= "\n ORDER BY  T1.order_regi_date    DESC";
        //$query .= "\n          ,T1.bun_dlvr_order_num DESC";
        //$query .= "\n          ,T1.order_common_seqno DESC";
        $query .= "\n    LIMIT 5";

        return $conn->Execute($query);
    }

    function selectOrderAfterInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name, ' - ' ,detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지') ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    /**
     * @brief 견적리스트 요약
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectEstiSummary($conn, $param) {
     
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $member_seqno = substr($param["member_seqno"], 1, -1);

        $query .= "\n SELECT  esti_seqno ";
        $query .= "\n        ,title ";
        $query .= "\n        ,state ";
        $query .= "\n        ,regi_date ";
        $query .= "\n   FROM  esti ";
        $query .= "\n  WHERE  member_seqno IN(" . $member_seqno . ")";
        $query .= "\n ORDER BY esti_seqno DESC ";
        $query .= "\n LIMIT 0, 5";

        return $conn->Execute($query);
    }
}
