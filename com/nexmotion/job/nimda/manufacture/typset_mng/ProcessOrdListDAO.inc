<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/manufacture/typset_mng/ProcessOrdListHTML.inc');

/**
 * @file ProcessViewDAO.inc
 *
 * @brief 생산 - 조판관리 - 생산공정관리 DAO
 */
class ProcessOrdListDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    // 생산 지시서 구분
    function selectProduceOrdDvs($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT count(A.produce_ord_seqno) AS cnt, sum(A.amt) AS tot_amt";
        $query .= "\n  FROM produce_ord AS A ";
        $query .= "\n  INNER JOIN sheet_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n WHERE date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND print_etprs = " .$param["ord_dvs"];
        $query .= "\n   AND dvs = " .$param["dvs"];

        return $conn->Execute($query);
    }

    // 생산 지시 구분
    function selectOrdDvs($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT DISTINCT(A.ord_dvs)";
        $query .= "\n  FROM produce_ord AS A ";
        $query .= "\n  INNER JOIN sheet_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n WHERE A.date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.date <= '" . $date . " 23:59:59'";

        if ($this->blankParameterCheck($param ,"ord_dvs")) {
            $query .= "\n  AND B.print_etprs = " .$param["ord_dvs"];
        }

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $val = substr($param["preset_cate"], 1, -1);
            $query .= "\n   AND A.dvs LIKE '" . $val . "%'";
        }

        return $conn->Execute($query);
    }

    // 생산 지시 구분
    function selectProduceListByPrinter($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, GROUP_CONCAT(DISTINCT D.after_name) AS after_name ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  LEFT JOIN order_after_history  AS D ON D.order_common_seqno = C.order_common_seqno AND D.basic_yn = 'N' ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('봉투','독판','전단')";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        $query .= "\n   GROUP BY A.typset_num ";
        $query .= "\n   ORDER BY A.typset_num ";
        return $conn->Execute($query);
    }

    // 생산 지시 구분
    function selectProduceListByEnvelope($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, D.depth1 as after_name, F.member_name, C.amt, G.stan_name, C.title, C.work_memo, C.count, C.order_detail, E.dlvr_way, F.dlvr_add_info ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  LEFT JOIN order_after_history  AS D ON D.order_common_seqno = C.order_common_seqno AND D.after_name = '접착' ";
        $query .= "\n  INNER JOIN order_dlvr  AS E ON E.order_common_seqno = C.order_common_seqno AND E.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS F ON F.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS G ON G.order_common_seqno = C.order_common_seqno ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('봉투')";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        if ($this->blankParameterCheck($param ,"after")) {
            if($param["after"] == "'없음'")
                $query .= "\n  AND D.after_name IS null";
            else
                $query .= "\n  AND D.after_name = " .$param["after"];
        }

        $query .= "\n   GROUP BY C.order_num ";
        $query .= "\n   ORDER BY  G.stan_name, F.member_name ";
        return $conn->Execute($query);
    }

    // 생산 지시 구분
    function selectProduceListByCommercial($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\n SELECT A.*, G.cut_size_wid, G.cut_size_vert,  F.member_name, C.amt, C.page_cnt, G.stan_name, C.work_memo, C.order_common_seqno ,C.title, C.count, C.order_detail, E.dlvr_way, F.dlvr_add_info ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  LEFT JOIN order_dlvr  AS E ON E.order_common_seqno = C.order_common_seqno AND E.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS F ON F.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS G ON G.order_common_seqno = C.order_common_seqno ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('독판','전단')";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        if ($this->blankParameterCheck($param ,"after")) {
            if($param["after"] == "'없음'")
                $query .= "\n  AND D.after_name IS null";
            else
                $query .= "\n  AND D.after_name = " .$param["after"];
        }

        $query .= "\n   GROUP BY C.order_num ";
        $query .= "\n   ORDER BY  G.stan_name, F.member_name ";
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

    function selectOrderAfterInfo2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name ,'-',detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지')";
        $query .= "\n       GROUP BY order_common_seqno ORDER BY after_name desc ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    // 생산 지시 구분
    function selectProduceListByAfter($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, GROUP_CONCAT(DISTINCT D.after_name) AS after_name, F.member_name, G.stan_name, C.title, C.count, C.order_detail, E.dlvr_way ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  INNER JOIN order_after_history  AS D ON D.order_common_seqno = C.order_common_seqno AND D.basic_yn = 'N' ";
        $query .= "\n  LEFT JOIN order_dlvr  AS E ON E.order_common_seqno = C.order_common_seqno AND E.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS F ON F.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS G ON G.order_common_seqno = C.order_common_seqno ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('전단','독판')";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        $query .= "\n   GROUP BY A.typset_num ";
        return $conn->Execute($query);
    }

    function selectProduceListByTypset($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, D.after_name, F.member_name, G.stan_name, C.title, C.count, E.invo_cpn as dlvr_add_info ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  LEFT JOIN order_after_history  AS D ON D.order_common_seqno = C.order_common_seqno AND D.basic_yn = 'N' ";
        $query .= "\n  INNER JOIN order_dlvr  AS E ON E.order_common_seqno = C.order_common_seqno AND E.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS F ON F.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS G ON G.order_common_seqno = C.order_common_seqno ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND E.dlvr_way = '01' ";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        if ($this->blankParameterCheck($param ,"dvs")) {
            $query .= "\n  AND A.dvs = " .$param["dvs"];
        } else {
            $query .= "\n   AND A.dvs IN ('전단','봉투')";
        }

        $query .= "\n   ORDER BY A.typset_num, F.member_name ";
        return $conn->Execute($query);
    }

    function selectProduceListByTypsetDirect($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, D.after_name, F.member_name, G.stan_name, C.title, C.count, F.dlvr_add_info, E.invo_cpn ";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n  INNER JOIN order_typset  AS B ON A.typset_num = B.typset_num ";
        $query .= "\n  INNER JOIN order_common  AS C ON C.order_num = B.order_num ";
        $query .= "\n  LEFT JOIN order_after_history  AS D ON D.order_common_seqno = C.order_common_seqno AND D.basic_yn = 'N' ";
        $query .= "\n  INNER JOIN order_dlvr  AS E ON E.order_common_seqno = C.order_common_seqno AND E.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS F ON F.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS G ON G.order_common_seqno = C.order_common_seqno ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND E.dlvr_way = '02' ";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        if ($this->blankParameterCheck($param ,"dvs")) {
            $query .= "\n  AND A.dvs = " .$param["dvs"];
        } else {
            $query .= "\n   AND A.dvs IN ('전단','봉투')";
        }

        $query .= "\n   ORDER BY F.dlvr_add_info ";
        return $conn->Execute($query);
    }

    // 생산 지시 구분
    function selectProduceListByPaper($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, CONCAT(SUM(print_amt), ' ' ,print_amt_unit) as amt";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('전단','봉투','독판')";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $query .= "\n  AND A.print_etprs = " .$param["print_etprs"];
        }

        $query .= "\n   GROUP BY A.paper_name, A.size ";
        return $conn->Execute($query);
    }

    function selectProduceListByNamecardSum($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\n SELECT typset_num, paper_name,print_amt, beforeside_tmpt + aftside_tmpt AS tmpt, honggak_yn, dvs, print_title, print_etprs, memo ";
        $query .= "\n FROM sheet_typset AS A  ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('명함','스티커','디지털') AND A.typset_num not like '%-05%' ";
        $query .= "\n   GROUP BY A.typset_num ";

        return $conn->Execute($query);
    }

    function selectProduceListByNamecard($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.*, CONCAT(SUM(print_amt), ' ' ,print_amt_unit) as amt, SUM(print_amt) print_amt";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n WHERE A.regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND A.regi_date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND A.dvs IN ('명함','스티커','디지털') AND A.typset_num not like '%-05%'";

        if ($this->blankParameterCheck($param ,"print_etprs")) {
            $val = substr($param["print_etprs"], 1, -1);
            if($val == "코팅명함") {
                $query .= "\n AND A.typset_num like '%-11%' ";
            } else if($val == "무코팅명함") {
                $query .= "\n AND A.typset_num like '%-12%' ";
            } else if($val == "수입지") {
                $query .= "\n AND A.typset_num like '%-13%' ";
            } else if($val == "DTP") {
                $query .= "\n AND A.typset_num like '%-15%' ";
            } else if($val == "스티커") {
                $query .= "\n AND (A.typset_num like '%-14%' OR A.typset_num like '%-16%') ";
            }
        }

        $query .= "\n   GROUP BY A.typset_num ";
        return $conn->Execute($query);
    }

    // 생산 구분
    function selectDvs($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT DISTINCT(dvs)";
        $query .= "\n  FROM produce_ord AS A ";
        $query .= "\n  INNER JOIN sheet_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n WHERE date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND date <= '" . $date . " 23:59:59'";

        if ($this->blankParameterCheck($param ,"ord_dvs")) {
            $query .= "\n  AND B.print_etprs = " .$param["ord_dvs"];
        }

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $val = substr($param["preset_cate"], 1, -1);
            $query .= "\n   AND dvs LIKE '" . $val . "%'";
        }

        return $conn->Execute($query);
    }

    // 생산 지시서 리스트
    function selectProduceOrdList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query  = "\nSELECT A.dvs, A.typset_num, A.paper, A.size, A.print_tmpt, A.amt, A.amt_unit, A.specialty_items ";
        $query .= "\n  FROM produce_ord AS A ";
        $query .= "\n  INNER JOIN sheet_typset AS B ON A.typset_num = B.typset_num ";
        $query .= "\n WHERE date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND date <= '" . $date . " 23:59:59'";
        $query .= "\n   AND dvs = " .$param["dvs"];

        if ($this->blankParameterCheck($param ,"ord_dvs")) {
            $query .= "\n  AND B.print_etprs = " .$param["ord_dvs"];
        }

        if ($this->blankParameterCheck($param ,"preset_cate")) {
            $val = substr($param["preset_cate"], 1, -1);
            $query .= "\n   AND dvs LIKE '" . $val . "%'";
        }

        return $conn->Execute($query);
    }

    // 작업지시서
    function selectTotalList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $date = substr($param["date"], 1, -1);
        $query = "\nSELECT dvs";
        $query .= "\n      ,print_amt";
        $query .= "\n  FROM sheet_typset AS A ";
        $query .= "\n WHERE regi_date >= '" . $date . " 00:00:00'";
        $query .= "\n   AND regi_date <= '" . $date . " 23:59:59'";
        $dvs = substr($param["dvs"], 1, -1);
        $query .= "\n   AND dvs LIKE '%" . $dvs . "%'";

        if ($this->blankParameterCheck($param ,"ord_dvs")) {
//            $query .= "\n  AND B.print_etprs = " .$param["ord_dvs"];
        }

        return $conn->Execute($query);
    }
}
?>
