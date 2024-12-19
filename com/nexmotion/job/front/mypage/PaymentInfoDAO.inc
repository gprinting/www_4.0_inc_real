<?
include_once(INC_PATH . "/com/nexmotion/job/front/common/MypageCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/front/mypage/PaymentInfoHtml.inc");

/**
 * @file OtoInqMngDAO.inc
 *
 * @brief 마이페이지 - 결제정보
 */
class PaymentInfoDAO extends MypageCommonDAO {

    function __construct() {
    }

    /**
     * @brief 결재내역 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaymentList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);

        if ($type === "COUNT") {
            $query  = "\n  SELECT  COUNT(*) AS cnt";
        } else {
            $query  = "\n  SELECT  A.deal_date";
            $query .= "\n         ,A.dvs";
            $query .= "\n         ,A.input_typ";
            $query .= "\n         ,A.depo_price";
            $query .= "\n         ,A.card_depo_price";
            $query .= "\n         ,A.pay_price";
            $query .= "\n         ,A.adjust_price";
            $query .= "\n         ,C.order_state" ;
            $query .= "\n         ,C.order_num" ;
            $query .= "\n         ,C.title " ;
            $query .= "\n         ,D.cont " ;
            $query .= "\n         ,A.cont cont2 " ;
        }
        $query .= "\n    FROM  member_pay_history AS A";
        $query .= "\n    INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n    LEFT JOIN order_common AS C ON A.order_num = C.order_num ";
        $query .= "\n    LEFT JOIN adjust AS D ON A.adjust_seqno = D.adjust_seqno ";
        $query .= "\n   WHERE  (C.order_state NOT IN ('1080','1120','1180') OR C.order_state IS NULL) AND (A.card_pay_price = 0 OR A.card_pay_price is null) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND A.dvs != '사용' AND (C.pay_way = '선입금' OR A.dvs = '입금증가' OR A.dvs = '입금조정') AND ((A.pay_price != 0 AND A.pay_price IS NOT NULL) OR (A.depo_price != 0 AND A.depo_price IS NOT NULL) OR (A.adjust_price != 0 AND A.adjust_price IS NOT NULL)) ";

        //구분
        if ($this->blankParameterCheck($param ,"dvs")) {
            $query .= "\n     AND  A.dvs = " . $param["dvs"];
        }

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

        if ($type == "SEQ") {
            $query .= "\nORDER BY A.deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);
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
        $query .= "\n         ,A.order_num";
        $query .= "\n         ,case when A.dvs = '배송비' then '' else B.order_detail end as order_detail";
        $query .= "\n         ,case when A.dvs = '배송비' then '' else B.amt end  as amt";
        $query .= "\n         ,case when A.dvs = '배송비' then '' else B.amt_unit_dvs end as amt_unit_dvs";
        $query .= "\n         ,B.count";
        $query .= "\n         ,case when A.dvs = '배송비' then concat('선불택배',' ',B.title) else B.title end  as title";
        $query .= "\n         ,A.sell_price";
        $query .= "\n         ,A.sale_price";
        $query .= "\n         ,A.pay_price, A.card_pay_price, A.dvs, A.card_depo_price, A.depo_price, A.adjust_price";
        $query .= "\n     FROM  member_pay_history AS A";
        $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180') OR B.order_state IS NULL) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND B.OPI_Date IS NULL AND (A.pay_price != 0 OR A.card_pay_price != 0) ";
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

        if ($type == "SEQ") {
            $query .= "\nORDER BY A.deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    function selectTransactionalInfoList2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);

        $query  = "\n  SELECT  SQL_CALC_FOUND_ROWS";
        $query .= "\n          deal_date";
        $query .= "\n         ,cont";
        $query .= "\n         ,order_num";
        $query .= "\n         ,dvs";
        $query .= "\n         ,sale_price";
        $query .= "\n         ,pay_price";
        $query .= "\n         ,card_pay_price";
        $query .= "\n         ,case when input_typ = '매출' then adjust_price else 0 end as sell_price";
        $query .= "\n         ,case when input_typ = '에누리' then adjust_price else 0 end as adjust_price";
        $query .= "\n     FROM  member_pay_history";
       // $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
       // $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180') OR B.order_state IS NULL) ";
        $query .= "\n     WHERE  member_seqno IN (" . $seqno . ") AND dvs='입금조정'";
    

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }  
  
        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($type == "SEQ") {
            $query .= "\nORDER BY deal_date DESC";
            $query .= "\n   LIMIT ". $s_num . ", " . $list_num;
        }

         return $conn->Execute($query);
    }


    /**
     * @brief 거래내역 총매출액,에누리,순매출액,입금액,일일잔액
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectTransactionPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);
        $query  = "\n  SELECT SUM(A.sell_price) AS sell_price";
        $query .= "\n        ,SUM(A.sale_price) AS sale_price";
        $query .= "\n        ,SUM(A.pay_price) AS pay_price";
        $query .= "\n        ,SUM(case when input_typ = '에누리' then adjust_price else 0 END ) AS adjust_price";
        $query .= "\n        ,SUM(case when input_typ = '매출' then adjust_price else 0 END ) AS sell_price2";
        $query .= "\n     FROM  member_pay_history AS A";
        $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180') OR B.order_state IS NULL) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND B.OPI_Date IS NULL ";

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

        return $conn->Execute($query);
    }


      function selectTransactionPrice2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);
        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);
        $query  = "\n  SELECT SUM(A.sell_price) AS sell_price";
        $query .= "\n        ,SUM(A.sale_price) AS sale_price";
        $query .= "\n        ,SUM(A.pay_price) AS pay_price";
        $query .= "\n        ,SUM(case when input_typ = '에누리' then adjust_price else 0 END ) AS adjust_price";
        $query .= "\n        ,SUM(case when input_typ = '매출' then adjust_price else 0 END ) AS sell_price2";
        $query .= "\n     FROM  member_pay_history AS A";
        $query .= "\n     LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n     WHERE (B.order_state NOT IN ('1080','1120', '1180') OR B.order_state IS NULL) ";
        $query .= "\n     AND  A.member_seqno IN (" . $seqno . ") AND B.OPI_Date IS NULL ";

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

        //return $conn->Execute($query);
        echo $query;
    }

     

    /**
     * @brief 거래내역 입금액,일일잔액
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDepoBalancePrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);
        if ($type == "BALANCE") {
            $query .= "\n  SELECT prepay_bal";
        } else {
            $query  = "\n  SELECT SUM(depo_price) AS depo_price, SUM(card_depo_price) AS card_depo_price";
        }
        $query .= "\n    FROM  member_pay_history AS A ";
        //$query .= "\n   WHERE  member_seqno =" . $seqno;
        $query .= "\n     WHERE 1 = 1 AND  A.member_seqno IN (" . $seqno . ") ";

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }  

        if ($type == "BALANCE") {
            $query .= "\nORDER BY deal_date DESC";
            $query .= "\n   LIMIT 1";
        }

        return $conn->Execute($query);
    }

    function selectDepoBalancePrice2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $type = substr($param["type"], 1, -1);
        $seqno = substr($param["member_seqno"], 1, -1);
        if ($type == "BALANCE") {
            $query .= "\n  SELECT prepay_bal";
        } else {
            $query  = "\n  SELECT SUM(depo_price) AS depo_price, SUM(card_depo_price) AS card_depo_price";
        }
        $query .= "\n    FROM  member_pay_history AS A ";
        //$query .= "\n   WHERE  member_seqno =" . $seqno;
        $query .= "\n     WHERE 1 = 1 AND  A.member_seqno IN (" . $seqno . ") ";

        //등록일
        if ($this->blankParameterCheck($param ,"from")) {
            $from = substr($param["from"], 1, -1);
            $query .= "\n     AND  deal_date >= '" . $from;
            $query .=" 00:00:00'";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $to = substr($param["to"], 1, -1);
            $query .= "\n     AND  deal_date <= '" . $to;
            $query .=" 23:59:59'";
        }  

        if ($type == "BALANCE") {
            $query .= "\nORDER BY deal_date DESC";
            $query .= "\n   LIMIT 1";
        }

        //return $conn->Execute($query);
        echo $query;
    }

    /**
     * @brief 해당 회원의 사업자 정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectLicenseInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["col"]  = " corp_name";
        $param["col"] .= ",crn";
        $param["col"] .= ",repre_name";
        $param["col"] .= ",addr";
        $param["col"] .= ",addr_detail";
        $param["col"] .= ",bc";
        $param["col"] .= ",tob";
        $param["table"] = "licensee_info";
        $param["where"]["member_seqno"] = $param["member_seqno"];

        $rs = $this->selectData($conn, $param);

        return $rs->fields;
    }
}
?>
