<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/07/21 엄준현 생성
 *=============================================================================
 */
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');

class SettleMngDAO extends BusinessCommonDAO {

    /**
     * @brief 결산관리 - 개인별 업체별 미수/매출액 담당자 영역 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = 페이징 변수
     *
     * @return 검색결과
     */
    public function selectSettleInfo($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn,
                                             $param,
                                             ["order" => true]);

        $query  = "\n SELECT  /* SettleMngDAO.inc - 영업-결산관리 담당자 총합/리스트 검색 */";
        if ($page > -1 && !$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.name";
        $query .= "\n        ,A.empl_seqno";
        $query .= "\n        ,SUM(IFNULL(C.net_sales_price, 0)) AS sum_net_price";
        $query .= "\n        ,SUM(IFNULL(C.depo_price, 0)) AS sum_depo_price";
        $query .= "\n        ,SUM(IFNULL(D.period_end_oa, 0)) AS sum_period_end_oa";
        $query .= "\n   FROM  empl AS A";

        $query .= "\n LEFT OUTER JOIN order_common AS B";
        $query .= "\n   ON A.name = B.receipt_mng";
        $query .= "\n  AND B.receipt_finish_date BETWEEN %s 00:00:00' AND %s 23:59:59'";
        if ($this->blankParameterCheck($param, "oper_sys")) {
            $query .= "\n  AND B.oper_sys = ";
            $query .= $param["oper_sys"];
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n  AND B.title LIKE '%%";
            $query .= substr($param["title"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n  AND B.cate_sortcode LIKE '";
            $query .= substr($param["cate_sortcode"], 1, -1) . "%%'";
        } 

        $query .= "\n LEFT JOIN member AS E";
        $query .= "\n   ON B.member_seqno = E.member_seqno";
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n  AND E.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "grade")) {
            $query .= "\n  AND E.grade = ";
            $query .= $param["grade"];
        }

        $query .= "\n LEFT JOIN day_sales_stats AS C";
        $query .= "\n   ON B.member_seqno = C.member_seqno";

        $query .= "\n LEFT JOIN day_sales_stats AS D";
        $query .= "\n   ON B.member_seqno = D.member_seqno";
        $query .= "\n  AND D.input_date = %s";

        $query .= "\n LEFT JOIN order_dlvr AS F";
        $query .= "\n   ON B.order_common_seqno = F.order_common_seqno";
        $query .= "\n  AND F.tsrs_dvs = '수신'";
        if ($this->blankParameterCheck($param, "dlvr_way")) {
            $query .= "\n  AND F.dlvr_way = ";
            $query .= $param["dlvr_way"];
        }
        if ($this->blankParameterCheck($param, "invo_cpn")) {
            $query .= "\n  AND F.invo_cpn = ";
            $query .= $param["invo_cpn"];
        }

        $query .= "\n  WHERE  A.cpn_admin_seqno = %s";
        if ($this->blankParameterCheck($param, "depar_code")) {
            $query .= "\n    AND  A.depar_code = ";
            $query .= $param["depar_code"];
        } else {
            $query .= "\n    AND  A.high_depar_code = ";
            $query .= $param["high_depar_code"];
        }
        if ($this->blankParameterCheck($param, "empl_seqno")) {
            $query .= "\n    AND  A.empl_seqno = ";
            $query .= $param["empl_seqno"];
        }

        if ($page > -1) {
            $query .= "\n  GROUP BY  A.name";
        }
        $order_by = '';

        if ($this->blankParameterCheck($param["order"], "sum_net_price")) {
            $order_by .= "\n            sum_net_price ";
            $order_by .= $param["order"]["sum_net_price"];
        }
        if ($this->blankParameterCheck($param["order"], "sum_depo_price")) {
            $order_by .= "\n            sum_depo_price ";
            $order_by .= $param["order"]["sum_depo_price"];
        }
        if ($this->blankParameterCheck($param["order"], "sum_period_end_oa")) {
            $order_by .= "\n            sum_period_end_oa ";
            $order_by .= $param["order"]["sum_period_end_oa"];
        }
        if (!empty($order_by)) {
            $query .= "\n  ORDER BY";
            $query .= $order_by;
        }

        if ($page > -1) {
            $query .= "\n  LIMIT  " . $page . ", 5";
        }

        $query  = sprintf($query, substr($param["from"], 0, -1)
                                , substr($param["to"], 0, -1)
                                , $param["input_date"]
                                , $param["cpn_admin_seqno"]
                                );

        $rs = $conn->Execute($query);
        
        return $rs;
    }

    /**
     * @brief 결산관리 - 개인별 업체별 미수/매출액 업체 영역 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $page  = 페이징 변수
     *
     * @return 검색결과
     */
    public function selectSettleInfoDetail($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn,
                                             $param,
                                             ["order" => true]);

        $query  = "\n SELECT  /* SettleMngDAO.inc - 영업-결산관리 담당자 총합/리스트 검색 */";
        if ($page > -1 && !$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         E.member_seqno";
        $query .= "\n        ,E.office_nick";
        $query .= "\n        ,E.member_typ";
        $query .= "\n        ,SUM(IFNULL(C.net_sales_price, 0)) AS sum_net_price";
        $query .= "\n        ,SUM(IFNULL(C.depo_price, 0)) AS sum_depo_price";
        $query .= "\n        ,SUM(IFNULL(D.period_end_oa, 0)) AS sum_period_end_oa";
        $query .= "\n        ,COUNT(*) AS div_count";
        $query .= "\n   FROM  empl AS A";

        $query .= "\n LEFT OUTER JOIN order_common AS B";
        $query .= "\n   ON A.name = B.receipt_mng";
        $query .= "\n  AND B.receipt_finish_date BETWEEN %s 00:00:00' AND %s 23:59:59'";
        if ($this->blankParameterCheck($param, "oper_sys")) {
            $query .= "\n  AND B.oper_sys = ";
            $query .= $param["oper_sys"];
        }
        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n  AND B.title LIKE '%%";
            $query .= substr($param["title"], 1, -1) . "%%'";
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n  AND B.cate_sortcode LIKE '";
            $query .= substr($param["cate_sortcode"], 1, -1) . "%%'";
        } 

        $query .= "\n LEFT JOIN member AS E";
        $query .= "\n   ON B.member_seqno = E.member_seqno";
        if ($this->blankParameterCheck($param, "member_typ")) {
            $query .= "\n  AND E.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param, "grade")) {
            $query .= "\n  AND E.grade = ";
            $query .= $param["grade"];
        }

        $query .= "\n LEFT JOIN day_sales_stats AS C";
        $query .= "\n   ON B.member_seqno = C.member_seqno";

        $query .= "\n LEFT JOIN day_sales_stats AS D";
        $query .= "\n   ON B.member_seqno = D.member_seqno";
        $query .= "\n  AND D.input_date = %s";


        $query .= "\n LEFT JOIN order_dlvr AS F";
        $query .= "\n   ON B.order_common_seqno = F.order_common_seqno";
        $query .= "\n  AND F.tsrs_dvs = '수신'";
        if ($this->blankParameterCheck($param, "dlvr_way")) {
            $query .= "\n  AND F.dlvr_way = ";
            $query .= $param["dlvr_way"];
        }
        if ($this->blankParameterCheck($param, "invo_cpn")) {
            $query .= "\n  AND F.invo_cpn = ";
            $query .= $param["invo_cpn"];
        }

        $query .= "\n  WHERE  A.cpn_admin_seqno = %s";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  E.member_seqno = ";
            $query .= $param["member_seqno"];
        }
        if ($this->blankParameterCheck($param, "depar_code")) {
            $query .= "\n    AND  A.depar_code = ";
            $query .= $param["depar_code"];
        } else {
            $query .= "\n    AND  A.high_depar_code = ";
            $query .= $param["high_depar_code"];
        }
        if ($this->blankParameterCheck($param, "empl_seqno")) {
            $query .= "\n    AND  A.empl_seqno = ";
            $query .= $param["empl_seqno"];
        }

        if ($page > -1) {
            $query .= "\n  GROUP BY  E.member_name";
        }

        $order_by = '';
        if ($this->blankParameterCheck($param["order"], "office_nick")) {
            $order_by .= "\n            E.office_nick ";
            $order_by .= $param["order"]["office_nick"];
        }
        if ($this->blankParameterCheck($param["order"], "member_typ")) {
            $order_by .= "\n            E.member_typ";
            $order_by .= $param["order"]["member_typ"];
        }
        if ($this->blankParameterCheck($param["order"], "sum_net_price")) {
            $order_by .= "\n            sum_net_price ";
            $order_by .= $param["order"]["sum_net_price"];
        }
        if ($this->blankParameterCheck($param["order"], "sum_depo_price")) {
            $order_by .= "\n            sum_depo_price ";
            $order_by .= $param["order"]["sum_depo_price"];
        }
        if ($this->blankParameterCheck($param["order"], "sum_period_end_oa")) {
            $order_by .= "\n            sum_period_end_oa ";
            $order_by .= $param["order"]["sum_period_end_oa"];
        }
        if ($this->blankParameterCheck($param["order"], "m1_sum_net")) {
            $order_by .= "\n            m1_sum_net ";
            $order_by .= $param["order"]["m1_sum_net"];
        }
        if ($this->blankParameterCheck($param["order"], "avg_sum_net")) {
            $order_by .= "\n            avg_sum_net ";
            $order_by .= $param["order"]["avg_sum_net"];
        }

        if (!empty($order_by)) {
            $query .= "\n  ORDER BY";
            $query .= $order_by;
        }

        if ($page > -1) {
            $query .= "\n  LIMIT  " . $page . ", 5";
        }

        $query  = sprintf($query, substr($param["from"], 0, -1)
                                , substr($param["to"], 0, -1)
                                , $param["input_date"]
                                , $param["cpn_admin_seqno"]
                                );

        $rs = $conn->Execute($query);
        
        return $rs;
    }

    /**
     * @brief 회원 기간별 일별 집계데이터 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    public function selectDaySalesStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT /* SettleMngDAO.inc - 영업-결산관리 매출/입금/기말미수 검색 */";
        $query .= "\n        IFNULL(A.net_sales_price, 0) AS net_sales_price";
        $query .= "\n       ,input_date";
        $query .= "\n  FROM  day_sales_stats AS A";
        $query .= "\n WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n   AND  member_seqno = ";
            $query .= $param["member_seqno"];
        }
        $query .= "\n   AND  %s <= A.input_date";
        $query .= "\n   AND  A.input_date <= %s";
        $query .= "\n ORDER  BY A.input_date DESC;";

        $query  = sprintf($query, $param["from"]
                                , $param["to"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 회원 사내닉네임 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     */
    public function selectMemberInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.member_name";
        $query .= "\n        ,A.office_nick";
        $query .= "\n        ,A.member_typ";
        $query .= "\n        ,B.loan_pay_promi_dvs";
        $query .= "\n   FROM  member AS A";
        $query .= "\n LEFT OUTER JOIN excpt_member AS B";
        $query .= "\n ON A.member_seqno = B.member_seqno ";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief empl_name 조회
     *        팀단위 검색을 하기 위함.
     * @param $conn  = connection identifier
     */
    function selectEmplTeamByName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
 
        $query  = "\n SELECT  A.name";
        $query .= "\n   FROM  empl AS A";
        $query .= "\n  WHERE  A.depar_code = %s";

        $query  = sprintf($query, $param["depar"]);
        
        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 매출액, 입금액 리스트 로드
     *
     * @writer montvert(2017.08.16)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 
     */
    function selectSettleSalesDepo($conn, $param, $page) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, 
                                             $param,
                                             ["order" => true]);

        $query  = "\n SELECT  /* 매출액, 입금액 리스트 쿼리 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         B.member_name";
        $query .= "\n       , A.deal_date";
        $query .= "\n       , A.dvs";
        $query .= "\n       , A.exist_prepay";
        $query .= "\n       , A.sell_price";
        $query .= "\n       , A.pay_price";
        $query .= "\n       , A.depo_price";
        $query .= "\n       , A.prepay_bal";
        $query .= "\n       , A.input_typ";
        $query .= "\n       , A.empl_name";
        $query .= "\n       , A.cont";

        $query .= "\n   FROM  member_pay_history AS A";
        $query .= "\n       , member AS B";

        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param, "cpn_admin_seqno")) {
            $query .= "\n    AND  B.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];
        }
        if ($this->blankParameterCheck($param, "from")) {
            $query .= "\n    AND  " . substr($param["from"], 0, -1);
            $query .= " 00:00:00' <= A.deal_date";
        }
        if ($this->blankParameterCheck($param, "to")) {
            $query .= "\n    AND  A.deal_date <= ";
            $query .= substr($param["to"], 0, -1) . " 23:59:59'";
        }
        // 팀단위 검색
        if ($this->blankParameterCheck($param ,"empl_name")) {
            $query .= "\n   AND  A.empl_name IN ( ";
            $query .= $param["empl_name"] . ")";
            // 해당 담당자만 검색
            if ($this->blankParameterCheck($param ,"empl")) {
                $query .= "\n   AND  A.empl_name = ";
                $query .= $param["empl"];
            }
        }
        // 배송정보 검색
        if ($this->blankParameterCheck($param ,"dlvr_typ")) {
            $query .= "\n   AND  B.dlvr_dvs = ";
            $query .= $param["dlvr_typ"];
            if ($this->blankParameterCheck($param ,"dlvr_code_arr")) {
                $query .= "\n   AND  B.dlvr_code IN ( ";
                $query .= $param["dlvr_code_arr"] . ")";
            }
            // 해당 담당자만 검색
            if ($this->blankParameterCheck($param ,"dlvr_code")) {
                $query .= "\n   AND  B.dlvr_code = ";
                $query .= $param["dlvr_code"];
            }
        }
        if ($this->blankParameterCheck($param ,"member_typ")) {
            $query .= "\n   AND  B.member_typ = ";
            $query .= $param["member_typ"];
        }
        if ($this->blankParameterCheck($param ,"member_grade")) {
            $query .= "\n   AND  B.grade = ";
            $query .= $param["member_grade"];
        }
        if ($this->blankParameterCheck($param ,"search_depo")) {
            $query .= "\n   AND  A.dvs LIKE ";
            $val = substr($param["search_depo"], 1, -1);
            $query .= "'%" . $val . "%' ";
        }
        if ($this->blankParameterCheck($param ,"dvs_detail")) {
            $query .= "\n   AND  A.dvs_detail LIKE ";
            $val = substr($param["dvs_detail"], 1, -1);
            $query .= "'%" . $val . "%' ";
        }
        $order_by = '';
        $order_by_arr = [];

        if ($this->blankParameterCheck($param["order"], "member_name")) {
            $order_by .= "\n            B.member_name ";
            $order_by .= $param["order"]["member_name"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "deal_date")) {
            $order_by .= "\n            A.deal_date ";
            $order_by .= $param["order"]["deal_date"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "dvs")) {
            $order_by .= "\n            A.dvs ";
            $order_by .= $param["order"]["dvs"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "sell_price")) {
            $order_by .= "\n            A.sell_price ";
            $order_by .= $param["order"]["sell_price"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "exist_prepay")) {
            $order_by .= "\n            A.exist_prepay ";
            $order_by .= $param["order"]["exist_prepay"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "pay_price")) {
            $order_by .= "\n            A.pay_price ";
            $order_by .= $param["order"]["pay_price"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "depo_price")) {
            $order_by .= "\n            A.depo_price ";
            $order_by .= $param["order"]["depo_price"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "prepay_bal")) {
            $order_by .= "\n            A.prepay_bal ";
            $order_by .= $param["order"]["prepay_bal"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "input_typ")) {
            $order_by .= "\n            A.input_typ ";
            $order_by .= $param["order"]["input_typ"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "empl_name")) {
            $order_by .= "\n            A.empl_name ";
            $order_by .= $param["order"]["empl_name"];
            $order_by_arr[] = $order_by;
        }
        if ($this->blankParameterCheck($param["order"], "cont")) {
            $order_by .= "\n            A.cont ";
            $order_by .= $param["order"]["cont"];
            $order_by_arr[] = $order_by;
        }
        if (!empty($order_by)) {
            $query .= "\n  ORDER BY";
            $query .= implode(",", $order_by_arr);
        }
        //$query .= "\n  ORDER BY A.member_pay_history_seqno DESC";
        $query .= "\n  LIMIT  " . $page . ", 5";

        $rs = $conn->Execute($query);

        return $rs;
    }


}
