<?
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/nimda/calcul_mng/settle/IncomeDataHTML.inc");

/**
 * @file IncomeDataDAO.inc
 *
 * @brief 정산관리 - 결산 - 수입자료 DAO
 */
class IncomeDataDAO extends NimdaCommonDAO {

    function __construct() {
    }

    /*
     * 금전출납부 수입자료 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectIncomeList($conn, $param)
    {
        $dvs = $param['dvs'];
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        if ($dvs == "SUM" || $dvs == "COUNT") {
            $query = "\n    SELECT COUNT(C.member_seqno) AS cnt ";
            $query .= "\n    , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri  ";
            $query .= "\n   , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales  ";
            $query .= "\n   , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit  ";
            $query .= "\n   , SUM(CASE WHEN A.input_typ = '기타' THEN A.adjust_price ELSE 0 END) etc_deposit ";
            $query .= "\n   , SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price ";
            $query .= "\n   , IFNULL(SUM(A.card_pay_price),0) AS card_pay_price , IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price  ";
        } else {
            $query = "\n    SELECT C.sell_channel ,SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price , IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price, C.member_name, C.id, A.member_seqno,  SUM(A.pay_price) + SUM(A.card_pay_price) AS all_sale_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
            $query .= "\n  , SUM(CASE WHEN D.deposit_dvs = 'cash' THEN (A.depo_price  + A.adjust_price) ELSE 0 END) cash_depo ";
        }
        $query .= "\n    FROM member_pay_history AS A";
        $query .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n    INNER JOIN member AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n    LEFT JOIN adjust AS D ON A.adjust_seqno = D.adjust_seqno ";
        $query .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param, "date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param, "date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param, "member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param, "cpn_admin_seqno")) {

            $query .= "\n           AND  C.sell_channel = ";
            $query .= $param["cpn_admin_seqno"];

        }
        if ($this->blankParameterCheck($param, "dlvr_dvs")) {

            if($param['dlvr_dvs'] == "'02'") {
                $query .= "\n           AND  C.direct_dlvr_yn = 'Y' ";
            }

        }

        if ($dvs == "SUM") {
            //if ($this->blankParameterCheck($param, "depo_path")) {
            //    if($param["depo_path"] == "'가상계좌'")
            //        $query .= "\n    AND A.depo_price > 0 ";
            //    if($param["depo_path"] == "'카드'")
            //        $query .= "\n    AND  (A.card_depo_price > 0 OR A.card_pay_price > 0) ";
            //    if($param["depo_path"] == "'기타'")
            //        $query .= "\n    AND  (A.adjust_price > 0) ";
            //}
        }
        else {
            $query .= "\n    GROUP BY A.member_seqno ";
            if ($this->blankParameterCheck($param, "depo_path")) {
                if($param["depo_path"] == "'가상계좌'")
                    $query .= "\n    HAVING depo_price > 0 ";
                if($param["depo_path"] == "'카드'")
                    $query .= "\n    HAVING card_depo_price > 0 OR card_pay_price > 0 ";
                if($param["depo_path"] == "'기타'")
                    $query .= "\n    HAVING adjust_price > 0 OR enuri > 0 ";
            }

            //limit 조건
            if($dvs == "SEQ") {
                $query .= "\n  ORDER BY all_sale_price DESC, card_depo_price DESC, depo_price DESC, adjust_price DESC";

                if ($this->blankParameterCheck($param, "start")
                    && $this->blankParameterCheck($param, "end")) {

                    $param["start"] = substr($param["start"], 1, -1);
                    $param["end"] = substr($param["end"], 1, -1);

                    $query .= "\n LIMIT " . $param["start"] . ",";
                    $query .= $param["end"];
                }
            }
        }
       // echo $query;
        $result = $conn->Execute($query);
        return $result;
    }

    /*
     * 금전출납부 수입자료 list Select
     * $conn : DB Connection
     * return : resultSet
     */
    function selectPeriodIncomeSum($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT C.sell_channel ,SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price , IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price   ";
        $query  .= "\n    ,SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ,SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ,SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit  ";
        $query  .= "\n    FROM member_pay_history AS A";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    INNER JOIN member AS C ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n           AND  C.sell_channel = ";
            $query .= $param["cpn_admin_seqno"];

        }

        $query  .= "\n    GROUP BY A.member_seqno ";
        $query .= "\n  ORDER BY pay_price DESC, card_depo_price DESC, depo_price DESC, adjust_price DESC";

        //limit 조건
        if ($this->blankParameterCheck($param ,"start")
            && $this->blankParameterCheck($param ,"end")) {

            $param["start"] = substr($param["start"], 1, -1);
            $param["end"] = substr($param["end"], 1, -1);

            $query .= "\n LIMIT " . $param["start"] . ",";
            $query .= $param["end"];
        }
        $result = $conn->Execute($query);

        return $result;
    }

    function selectDayIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT IFNULL(A.pay_price, 0) pay_price, IFNULL(A.card_pay_price, 0) card_pay_price, IFNULL(A.adjust_price, 0) adjust_price,IFNULL(A.depo_price, 0) depo_price,IFNULL(A.card_depo_price, 0) card_depo_price, A.cont a_cont, D.cont d_cont, B.order_detail, B.pay_way, E.card_cpn, E.card_num  ";
        $query  .= "\n    FROM member_pay_history AS A";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    INNER JOIN member AS C ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN adjust AS D ON A.adjust_seqno = D.adjust_seqno ";
        $query  .= "\n    LEFT JOIN member_pay_history_card AS E ON A.member_pay_history_seqno = E.member_pay_history_seqno ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL AND (A.pay_price != 0 OR A.card_pay_price != 0)";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date")) {
            $from = substr($param["date"], 1, -1);
            $query .= "\n           AND  A.deal_date LIKE '";
            $query .= $from . "%%'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n           AND  C.sell_channel = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //$query  .= "\n    GROUP BY A.member_seqno ";
        //$query .= "\n  ORDER BY pay_price DESC, card_depo_price DESC, depo_price DESC, adjust_price DESC";


        $result = $conn->Execute($query);

        return $result;
    }

    function selectByWeekIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT CONCAT(DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(1-DAYOFWEEK(deal_date)) DAY),'%Y-%m-%d'), '~', DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(7-DAYOFWEEK(deal_date)) DAY),'%Y-%m-%d')) AS date_range,   ";
        $query  .= "\n    SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price  ";
        $query  .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
        $query  .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
        $query  .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        $query  .= "\n  ,  IFNULL(SUM(A.depo_price),0) AS depo_price ";
        $query  .= "\n    FROM member AS C ";
        $query  .= "\n    LEFT JOIN member_pay_history AS A ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL  ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        $from = str_replace('-', '', substr($param["date_from"], 1, -1));
        $to = str_replace('-', '', substr($param["date_to"], 1, -1));
        $query .= "\n     AND date_format(deal_date,'%Y%m%d') BETWEEN '" . $from . "' AND '" . $to ."' ";

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        $query  .= "\n    GROUP BY CONCAT(YEAR(deal_date), '/', WEEK(deal_date)) ";

        $result = $conn->Execute($query);

        return $result;
    }

    function selectByMonthIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT CONCAT(DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(1-DAYOFMONTH(deal_date)) DAY),'%Y-%m-%d'), '~', DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(31-DAYOFMONTH(deal_date)) DAY),'%Y-%m-%d')) AS date_range,     ";
        $query  .= "\n    SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price,  ";
        $query  .= "\n    IFNULL(SUM(A.depo_price),0) AS depo_price ";
        $query  .= "\n    FROM member AS C ";
        $query  .= "\n    LEFT JOIN member_pay_history AS A ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL  ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        $from = str_replace('-', '', substr($param["date_from"], 1, -1));
        $to = str_replace('-', '', substr($param["date_to"], 1, -1));
        $query .= "\n     AND date_format(deal_date,'%Y%m%d') BETWEEN '" . $from . "' AND '" . $to ."' ";

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        $query  .= "\n    GROUP BY CONCAT(YEAR(deal_date), '/', MONTH(deal_date)) ";
        $query  .= "\n    ORDER BY deal_date ";
        $result = $conn->Execute($query);

        return $result;
    }

    function selectByYearIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT CONCAT(DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(1-DAYOFYEAR(deal_date)) DAY),'%Y-%m-%d'), '~', DATE_FORMAT(DATE_ADD(deal_date, INTERVAL(365-DAYOFYEAR(deal_date)) DAY),'%Y-%m-%d')) AS date_range,   ";
        $query  .= "\n    SUM(A.pay_price) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price,  ";
        $query  .= "\n    IFNULL(SUM(A.depo_price),0) AS depo_price ";
        $query  .= "\n    FROM member AS C ";
        $query  .= "\n    LEFT JOIN member_pay_history AS A ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL  ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        $from = str_replace('-', '', substr($param["date_from"], 1, -1));
        $to = str_replace('-', '', substr($param["date_to"], 1, -1));
        $query .= "\n     AND date_format(deal_date,'%Y%m%d') BETWEEN '" . $from . "' AND '" . $to ."' ";

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        $query  .= "\n    GROUP BY CONCAT(YEAR(deal_date)) ";

        $result = $conn->Execute($query);

        return $result;
    }

    function selectByDayIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT DATE_FORMAT(deal_date,'%Y-%m-%d') AS date_range, C.member_name,  ";
        $query  .= "\n   IFNULL(SUM(A.pay_price), 0) AS pay_price, IFNULL(SUM(A.sale_price),0) AS use_point, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price,    ";
        $query  .= "\n    IFNULL(SUM(A.depo_price),0) AS depo_price, C.id,SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ,SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ,SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit ";
        $query  .= "\n    FROM member AS C ";
        $query  .= "\n    LEFT JOIN member_pay_history AS A ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL  ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        $from = str_replace('-', '', substr($param["date_from"], 1, -1));
        $to = str_replace('-', '', substr($param["date_to"], 1, -1));
        $query .= "\n     AND date_format(deal_date,'%Y%m%d') BETWEEN '" . $from . "' AND '" . $to ."' ";

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        $query  .= "\n    GROUP BY CONCAT(YEAR(deal_date), '/', WEEK(deal_date), '/', DAY(deal_date)) ";
        $query  .= "\n    ORDER BY deal_date desc ";


        //echo $query;

        $result = $conn->Execute($query);

        return $result;
    }

    function selectByDayIncomeList2($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT DATE_FORMAT(deal_date,'%Y-%m-%d') AS date_range, C.member_name,  ";
        $query  .= "\n   IFNULL(SUM(A.pay_price), 0) AS pay_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price,    ";
        $query  .= "\n    IFNULL(SUM(A.depo_price),0) AS depo_price, C.id,SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ,SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ,SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit ";
        $query  .= "\n    FROM member AS C ";
        $query  .= "\n    LEFT JOIN member_pay_history AS A ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    WHERE 1 = 1 AND ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL  ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";

        $from = str_replace('-', '', substr($param["date_from"], 1, -1));
        $to = str_replace('-', '', substr($param["date_to"], 1, -1));
        $query .= "\n     AND date_format(deal_date,'%Y%m%d') BETWEEN '" . $from . "' AND '" . $to ."' ";

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        $query  .= "\n    GROUP BY CONCAT(YEAR(deal_date), '/', WEEK(deal_date), '/', DAY(deal_date)) ";
        $query  .= "\n    ORDER BY deal_date asc ";

        echo $query;
       // $result = $conn->Execute($query);

       // return $result;
    }

    function selectMemberDepositList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT A.deal_num, A.deal_date, B.deal_date as adjust_date, A.dvs, (IFNULL(A.depo_price,0) + IFNULL(A.adjust_price,0)) depo_price, A.card_depo_price, A.cont, C.member_name ";
        $query  .= "\n   ,B.deposit_dvs, B.card_kind, B.card_inst_months, B.card_num, B.card_approve_num, B.card_approve_date";
        $query  .= "\n    FROM member_pay_history AS A ";
        $query  .= "\n    LEFT JOIN adjust AS B ON A.adjust_seqno = B.adjust_seqno ";
        $query  .= "\n    INNER JOIN member AS C ON C.member_seqno = A.member_seqno ";
        $query  .= "\n    WHERE A.member_seqno = %s AND A.dvs = '입금증가' ";

        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        if ($this->blankParameterCheck($param ,"dvs")) {

            $query .= "\n           AND  B.deposit_dvs = " . $param["dvs"];
        }
        $query  .= "\n    ORDER BY  adjust_date desc, deal_date desc ";
        $query  = sprintf($query
            ,$param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;
    }

    function selectMemberEnuriList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT A.deal_date, A.dvs, A.adjust_price, B.cont, A.order_num ";
        $query  .= "\n    FROM member_pay_history AS A ";
        $query  .= "\n    INNER JOIN adjust AS B ON A.adjust_seqno = B.adjust_seqno ";
        $query  .= "\n    WHERE A.member_seqno = %s AND (A.dvs = '입금증가' OR A.dvs = '입금조정') AND A.input_typ = '에누리' ";

        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }
        $query  .= "\n    ORDER BY  A.member_pay_history_seqno desc ";

        $query  = sprintf($query
            ,$param["member_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectMemberPayPriceList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT A.deal_date, A.dvs, (IFNULL(A.pay_price,0) + IFNULL(A.adjust_price,0)) adjust_price, B.cont, A.order_num ";
        $query  .= "\n    FROM member_pay_history AS A ";
        $query  .= "\n    INNER JOIN adjust AS B ON A.adjust_seqno = B.adjust_seqno ";
        $query  .= "\n    WHERE A.member_seqno = %s AND (A.dvs = '입금조정' OR A.dvs = '매출증가') AND A.input_typ = '매출' ";

        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }
        $query  .= "\n    ORDER BY  A.member_pay_history_seqno desc ";

        $query  = sprintf($query
            ,$param["member_seqno"]);
        $result = $conn->Execute($query);
        
        return $result;
    }

    function selectMemberPayPriceList2($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT A.deal_date, A.dvs, (IFNULL(A.pay_price,0) + IFNULL(A.adjust_price,0)) adjust_price, B.cont, A.order_num ";
        $query  .= "\n    FROM member_pay_history AS A ";
        $query  .= "\n    INNER JOIN adjust AS B ON A.adjust_seqno = B.adjust_seqno ";
        $query  .= "\n    WHERE A.member_seqno = %s AND (A.dvs = '입금조정' OR A.dvs = '매출증가') AND A.input_typ = '매출' ";

        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }
        $query  .= "\n    ORDER BY  A.member_pay_history_seqno desc ";

        $query  = sprintf($query
            ,$param["member_seqno"]);
        echo $query;
    }


    function selectMemberMemo($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT * ";
        $query  .= "\n    FROM member_memo AS A ";
        $query  .= "\n    WHERE member_seqno = %s AND is_deleted = 'N'  ";

        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.insert_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.insert_date <= '";
            $query .= $to . " 23:59:59'";
        }

        $query  .= "\n    ORDER BY  insert_date desc ";
        $query  = sprintf($query
            ,$param["member_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectMemberInfo($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT * ";
        $query  .= "\n    FROM member AS A ";
        $query  .= "\n    INNER JOIN order_common AS B ON A.member_seqno = B.member_seqno AND depo_finish_date IS NOT NULL  AND order_state NOT IN ('1080','1120', '1180','1220')  ";
        $query  .= "\n    WHERE 1 = 1 ";
        //$query  .= "\n    AND B.OPI_Date IS NULL ";


        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }
        $query .= "\n  ORDER BY order_common_seqno DESC LIMIT 1 ";
        $result = $conn->Execute($query);

        return $result;
    }

    //C.member_name, A.member_seqno,MAX(B.depo_finish_date) AS depo_finish_date,C.cell_num, C.tel_num, C.fax_num, C.id



    function selectWithDraw($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  SUM(A.pay_price) as pay_price, B.member_name";
        $query  .= "\n    FROM order_common AS A, member AS B";
        $query  .= "\n    WHERE A.member_seqno = B.member_seqno AND 1 = 1 ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  order_regi_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  order_regi_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }

        $rs = $conn->Execute($query);
        return $rs;
    }

    /**
     * @brief 거래내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectTransactionalInfoList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);

        $query  = "\n  SELECT  SQL_CALC_FOUND_ROWS";
        $query .= "\n          A.deal_date";
        $query .= "\n         ,A.deal_num";
        $query .= "\n         ,A.order_num";
        $query .= "\n         ,B.order_detail";
        $query .= "\n         ,B.amt";
        $query .= "\n         ,B.amt_unit_dvs";
        $query .= "\n         ,A.card_pay_price";
        $query .= "\n         ,B.count";
        $query .= "\n         ,B.title";
        $query .= "\n         ,A.sell_price";
        $query .= "\n         ,A.sale_price";
        $query .= "\n         ,A.pay_price, A.dvs, A.card_depo_price, A.depo_price, A.adjust_price";
        $query .= "\n         ,C.input_dvs";
        $query .= "\n         ,C.input_dvs_detail";
        $query .= "\n         ,C.cont";
        $query .= "\n         ,D.card_cpn";
        $query .= "\n         ,D.aprvl_num";
        $query .= "\n         ,D.card_num, C.deposit_dvs";
        $query .= "\n     FROM  member_pay_history AS A";
        $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n     LEFT JOIN adjust AS C ON A.adjust_seqno = C.adjust_seqno ";
        $query .= "\n     LEFT JOIN member_pay_history_card AS D ON A.member_pay_history_seqno = D.member_pay_history_seqno ";
        $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND B.OPI_Date IS NULL AND (dvs != '매출증가' OR B.pay_way = '카드') AND dvs != '배송비' ";
        //$query .= "\n     AND  A.member_seqno IN (" . $seqno . ")  ";

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  A.deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  A.deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        if ($this->blankParameterCheck($param ,"input_dvs")) {
            $query .="\n C.input_dvs = " . $param["input_dvs"];
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($type == "SEQ") {
            $query .= "\nORDER BY A.deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        } else {
            $query .= "\nORDER BY A.deal_date DESC";
        }

        return $conn->Execute($query);
    }

    function selectTotalDayList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);

        $query  = "\n  SELECT  SQL_CALC_FOUND_ROWS";
        $query .= "\n          A.deal_date, A.member_seqno";
        $query .= "\n         ,SUM(A.pay_price) pay_price, SUM(IFNULL(A.card_pay_price,0)) card_pay_price, SUM(A.card_depo_price) card_depo_price, SUM(A.depo_price) depo_price, SUM(A.adjust_price) adjust_price ";
        $query .= "\n     FROM  member_pay_history AS A";
        $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND B.OPI_Date IS NULL ";
        //$query .= "\n     AND  A.member_seqno IN (" . $seqno . ")  ";

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  A.deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  A.deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);
        $query .= "\n   GROUP BY date_format(A.deal_date,'%Y-%m-%d') ";
        if ($type == "SEQ") {
            $query .= "\nORDER BY A.deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);
    }

    function selectWithDrawSum($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    SELECT  SUM(A.depo_price) as depo_price, SUM(A.card_depo_price) as card_depo_price";
        $query  .= "\n    FROM member_pay_history AS A";
        $query  .= "\n    WHERE 1 = 1 ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = $param["date_from"];
            $query .= "\n           AND  deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = $param["date_to"];
            $query .= "\n           AND  deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        return $conn->Execute($query);
    }

    function selectSellPriceSum($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    SELECT IFNULL(SUM(A.pay_price),0) pay_price, IFNULL(SUM(A.card_pay_price),0) card_pay_price ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        $query  .= "\n    FROM member_pay_history AS A";
        $query  .= "\n    INNER JOIN order_common AS B ON A.order_num = B.order_num ";
        $query  .= "\n    INNER JOIN member AS C ON A.member_seqno = C.member_seqno ";
        $query  .= "\n    WHERE ((B.order_state NOT IN ('1080','1120', '1180','1220') OR B.order_state IS NULL)) AND B.OPI_Date IS NULL ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = $param["date_from"];
            $query .= "\n           AND  A.deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = $param["date_to"];
            $query .= "\n           AND  A.deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        return $conn->Execute($query);
    }

    function insertMemberMemo($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    INSERT INTO member_memo ";
        $query  .= "\n    (member_seqno, memo, insert_date)";
        $query  .= "\n    VALUES ";
        $query  .= "\n    (%s, %s, %s) ";

        $query  = sprintf($query
            ,$param["member_seqno"]
            ,$param["member_memo"]
            ,$param["memo_date"]);
        return $conn->Execute($query);
    }

    function removeMemberMemo($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        //$param = $this->parameterArrayEscape($conn, $param);
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    UPDATE member_memo ";
        $query  .= "\n   SET is_deleted = 'Y' ";
        $query  .= "\n   WHERE member_memo_seqno IN (%s)  ";


        $query  = sprintf($query
            ,$param["idx"]);
        return $conn->Execute($query);
    }

    function selectDeliveryPriceSum($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    SELECT IFNULL(SUM(A.pay_price),0) pay_price";
        $query  .= "\n    FROM member_pay_history AS A  ";
        $query  .= "\n    INNER JOIN member AS B ON A.member_seqno = B.member_seqno  ";
        $query  .= "\n    WHERE A.dvs = '배송비' ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = $param["date_from"];
            $query .= "\n           AND  deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = $param["date_to"];
            $query .= "\n           AND  deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        return $conn->Execute($query);
    }

    function selectAdjustPriceSum($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        //$param['member_seqno'] = $this->parameterArrayEscape($conn, $param['member_seqno']);

        $query  = "\n    SELECT IFNULL(SUM(A.adjust_price),0) adjust_price";
        $query  .= "\n    FROM member_pay_history AS A  ";
        $query  .= "\n    INNER JOIN member AS B ON A.member_seqno = B.member_seqno  ";
        $query  .= "\n    WHERE A.dvs = '입금조정' ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = $param["date_from"];
            $query .= "\n           AND  deal_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = $param["date_to"];
            $query .= "\n           AND  deal_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        return $conn->Execute($query);
    }

    function selectIncomeSum($conn, $param)
    {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT  SUM(A.sales_price), SUM(A.adjust_price)";
        $query .= "\n    FROM day_settle AS A, member AS B";
        $query .= "\n    WHERE A.member_seqno = B.member_seqno ";

        $rs = $conn->Execute($query);
        return $rs;
    }

    /*
     * 금전출납부 수입자료 list Count
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function countIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    select SUM(cnt) as sum from (SELECT count(*) AS cnt ";
        $query  .= "\n    FROM day_settle AS A, member AS B";
        $query  .= "\n    WHERE A.member_seqno = B.member_seqno ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  update_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  update_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }
        $query  .= "\n    GROUP BY B.member_seqno ";
        $query .= "\n  ORDER BY B.member_seqno DESC) as a group by cnt";

        /*
        $query  = "\n    SELECT  count(*) cnt";
        $query .= "\n      FROM  cashbook A";
        $query .= "\n      LEFT OUTER JOIN member B";
        $query .= "\n        ON  A.member_seqno = B.member_seqno";
        $query .= "\n     WHERE  (dvs = 'income'";
        $query .= "\n        OR   dvs = 'trsf_income')";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {

            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.regi_date >= '";
            $query .= $from . " 00:00:00'";

        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {

            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.regi_date <= '";
            $query .= $to . " 23:59:59'";

        }
*/
        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 금전출납부 수입자료 list Count
     * $conn : DB Connection
     * return : resultSet
     */
    function countPeriodIncomeList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    select SUM(cnt) as sum from (SELECT count(*) AS cnt ";
        $query  .= "\n    FROM day_settle AS A, member AS B";
        $query  .= "\n    WHERE A.member_seqno = B.member_seqno ";

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {
            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  update_date >= '";
            $query .= $from . " 00:00:00'";
        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {
            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  update_date <= '";
            $query .= $to . " 23:59:59'";
        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }
        $query  .= "\n    GROUP BY B.member_seqno ";
        $query .= "\n  ORDER BY B.member_seqno DESC) as a group by cnt";

        /*
        $query  = "\n    SELECT  count(*) cnt";
        $query .= "\n      FROM  cashbook A";
        $query .= "\n      LEFT OUTER JOIN member B";
        $query .= "\n        ON  A.member_seqno = B.member_seqno";
        $query .= "\n     WHERE  (dvs = 'income'";
        $query .= "\n        OR   dvs = 'trsf_income')";

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {

            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.regi_date >= '";
            $query .= $from . " 00:00:00'";

        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {

            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.regi_date <= '";
            $query .= $to . " 23:59:59'";

        }
*/
        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 이체수입,이체지출 합계 가져오기 Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectIncomeSumPrice($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1,-1);

        $query  = "\n    SELECT";
        $query .= "\n            SUM(income_price)  AS income";
        $query .= "\n           ,SUM(trsf_income_price)   AS trsf_income";
        $query .= "\n      FROM  cashbook A";
        $query .= "\n      LEFT OUTER JOIN member B";
        $query .= "\n        ON  A.member_seqno = B.member_seqno";
        $query .= "\n     WHERE  (dvs = 'income'";
        $query .= "\n        OR   dvs = 'trsf_income')";

        if ($this->blankParameterCheck($param ,"sum_dvs")) {

            $query .= "\n       AND  A.depo_withdraw_path = ";
            $query .= $param["sum_dvs"];

        }

        //판매채널이 있을때
        if ($this->blankParameterCheck($param ,"cpn_admin_seqno")) {

            $query .= "\n            AND  A.cpn_admin_seqno = ";
            $query .= $param["cpn_admin_seqno"];

        }

        //회원 일련번호가 있을때
        if ($this->blankParameterCheck($param ,"member_seqno")) {

            $query .= "\n           AND  A.member_seqno = ";
            $query .= $param["member_seqno"];

        }

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param ,"depo_path")) {

            $query .= "\n            AND  A.depo_withdraw_path = ";
            $query .= $param["depo_path"];

        }

        //입출금경로 상세가 있을때
        if ($this->blankParameterCheck($param ,"depo_path_detail")) {

            $query .= "\n            AND  A.depo_withdraw_path_detail = ";
            $query .= $param["depo_path_detail"];

        }

        //시작날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_from")) {

            $from = substr($param["date_from"], 1, -1);
            $query .= "\n           AND  A.regi_date >= '";
            $query .= $from . " 00:00:00'";

        }

        //종료날짜가 있을때
        if ($this->blankParameterCheck($param ,"date_to")) {

            $to = substr($param["date_to"], 1, -1);
            $query .= "\n           AND  A.regi_date <= '";
            $query .= $to . " 23:59:59'";

        }

        $result = $conn->Execute($query);

        return $result;
    }


    /*
     * 입출금 상세 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectPathDetail($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    SELECT  B.name";
        $query .= "\n      FROM  depo_withdraw_path A";
        $query .= "\n           ,depo_withdraw_path_detail B";
        $query .= "\n     WHERE  A.depo_withdraw_path_seqno = ";
        $query .= "B.depo_withdraw_path_seqno";

        //입출금경로가 있을때
        if ($this->blankParameterCheck($param, "path")) {
            $query .= "\n       AND A.name = " . $param["path"];
        }

        $result = $conn->Execute($query);

        return $result;
    }



}
?>
