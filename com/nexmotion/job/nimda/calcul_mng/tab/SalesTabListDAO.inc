<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/calcul_mng/tab/SalesTabHTML.inc');

/**
 * @file SalesTabListDAO.inc
 *
 * @brief 정산 - 계산서 - 매출계산서리스트 DAO
 */


class SalesTabListDAO extends NimdaCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }
 
    /**
     * @brief 매출계산서 대기리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPublicStandByList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt , IFNULL(SUM(A.pay_price),0) AS pay_price";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num, B.cell_num, B.sell_channel,F.change_price,A.public_dvs, ";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n  LEFT JOIN tax_change_price AS F ON A.member_seqno = F.member_seqno and A.pay_year = F.member_year collate utf8_general_ci and A.pay_mon = F.member_mon collate utf8_general_ci ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date is null ";
        $query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR (A.public_dvs = '미발행' AND (A.dvs = '입금조정' OR A.dvs = '입금증가') AND (A.public_state = '대기' OR A.public_state = ''))) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";
        //$query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR (A.dvs = '입금조정' OR A.dvs = '입금증가')) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";
 
        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.sell_channel = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //업체명
        if ($this->blankParameterCheck($param ,"corp_name")) {

            $corp_name = substr($param["corp_name"], 1, -1); 
            $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }
        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs != "SUM") {
            $query .= "\n   GROUP BY B.member_seqno";
            //$query .= "\n   HAVING pay_price > 0 ";
        }
        $query .= "\n   HAVING pay_price > 0 ";
        $query .= "\n   ORDER BY B.sell_channel Desc, pay_price desc ";
        if ($dvs == "SEQ") {

            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);

    }

    function testQueryString($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

       if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt , IFNULL(SUM(A.pay_price),0) AS pay_price";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num, B.cell_num, B.sell_channel,F.change_price,A.public_dvs, ";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n  LEFT JOIN tax_change_price AS F ON A.member_seqno = F.member_seqno ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date is null ";
        $query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR (A.public_dvs = '미발행' AND (A.dvs = '입금조정' OR A.dvs = '입금증가') AND (A.public_state = '대기' OR A.public_state = ''))) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";
        //$query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR (A.dvs = '입금조정' OR A.dvs = '입금증가')) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";
 
        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.sell_channel = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //업체명
        if ($this->blankParameterCheck($param ,"corp_name")) {

            $corp_name = substr($param["corp_name"], 1, -1); 
            $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }
        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs != "SUM") {
            $query .= "\n   GROUP BY B.member_seqno";
            //$query .= "\n   HAVING pay_price > 0 ";
        }
        $query .= "\n   HAVING pay_price > 0 ";
        $query .= "\n   ORDER BY B.sell_channel Desc, pay_price desc ";
        if ($dvs == "SEQ") {

            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        echo $query;

    }

    function selectAllPayprice2($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num, B.cell_num, B.sell_channel, A.public_dvs, ";
        $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date IS NULL ";
        $query .= "\n AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        if ($dvs != "SUM") {
            $query .= "\n   GROUP BY B.member_seqno";
        }

        if($param['kind'] == "'card or cash'")
            $query .= "\n   HAVING pay_price > 0 OR card_pay_price > 0 ";
        else if($param['kind'] == "'card'")
            $query .= "\n   HAVING card_pay_price > 0 ";
        else
            $query .= "\n   HAVING pay_price > 0 ";

        return $conn->Execute($query);
    }

    function selectAllPayprice3($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num, B.cell_num, B.sell_channel, A.public_dvs, ";
        $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date IS NULL ";
        $query .= "\n AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";
        $query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR (A.public_dvs = '미발행' AND (A.dvs = '입금조정' OR A.dvs = '입금증가')) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        if ($dvs != "SUM") {
            $query .= "\n   GROUP BY B.member_seqno";
        }
        $query .= "\n   HAVING pay_price > 0 ";
        return $conn->Execute($query);
    }

    function selectCardpayList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(DISTINCT A.member_seqno) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num, B.cell_num, B.sell_channel, ";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.card_depo_price),0) AS card_depo_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL)  ";
        $query .= "\n AND (D.pay_way = '카드' OR (A.dvs = '입금증가' AND A.card_depo_price != 0)) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751)  ";

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.sell_channel = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") {

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //업체명
        if ($this->blankParameterCheck($param ,"corp_name")) {

            $corp_name = substr($param["corp_name"], 1, -1);
            $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\n   GROUP BY B.member_seqno";
            //$query .= "\n   HAVING pay_price > 0 ";

        }
        $query .= "\n   ORDER BY B.sell_channel Desc, card_pay_price desc";
        if ($dvs == "SEQ") {

            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }




    function selectDepoPrice($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT IFNULL(SUM(depo_price),0) depo_price, IFNULL(SUM(card_depo_price),0) card_depo_price ";
        $query .= "\n FROM member_pay_history AS A ";
        $query .= "\n WHERE A.member_seqno = '" . $param["member_seqno"] . "'";
        $query .= "\n   AND A.deal_date LIKE '" . $param['deal_date'] . "%'";
        //$query = sprintf($query, $param["member_seqno"]);
        return $conn->Execute($query);
    }

    function selectAllPayPrice($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT A.public_dvs, A.public_state, IFNULL(SUM(pay_price),0) pay_price, IFNULL(SUM(card_pay_price),0) card_pay_price ";
        $query .= "\n   , IFNULL(SUM(depo_price),0) depo_price, IFNULL(SUM(card_depo_price),0) card_depo_price , IFNULL(SUM(adjust_price),0) adjust_price  ";
        $query .= "\n FROM member_pay_history AS A ";
        $query .= "\n WHERE A.member_seqno = '" . $param["member_seqno"] . "'";
        $query .= "\n   AND A.deal_date LIKE '" . $param['deal_date'] . "%'";
        $query .= "\n AND A.public_dvs != '' ";
        $query .= "\n GROUP BY A.public_dvs, A.public_state ";
        //$query = sprintf($query, $param["member_seqno"]);
        return $conn->Execute($query);
    }

    /**
     * @brief 매출계산서 대기리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPublicStandByListExcel($conn, $param) {

    //커넥션 체크
    if ($this->connectionCheck($conn) === false) {
        return false;
    }

    //인젝션 어택 방지
    $param = $this->parameterArrayEscape($conn, $param);
    $dvs = substr($param["dvs"], 1, -1);

    if ($dvs == "COUNT") {
        $query  ="\nSELECT  COUNT(DISTINCT B.member_seqno) AS cnt ";
    } else if ($dvs == "SEQ") {
        $query  = "\nSELECT B.sell_channel, A.public_dvs, A.public_state, B.member_seqno, B.cell_num, B.tel_num, B.member_name, C.crn, C.corp_name, B.tel_num, A.member_seqno, ";
        $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price ,IFNULL(SUM(A.card_depo_price),0) AS card_depo_price ";
        $query .= "\n  , SUM(CASE WHEN A.deal_num IS NULL then A.card_depo_price  ELSE 0 END) card_depo_price1 ";
        $query .= "\n  , SUM(CASE WHEN A.deal_num IS NOT NULL then A.card_depo_price  ELSE 0 END) card_depo_price2 ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit ";
        $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' THEN A.depo_price ELSE 0 END) adjust_etc ";
    } else if ($dvs == "SUM") {
        $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        $query  .= "\n, IFNULL(SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END),0) AS enuri ";
    }
    $query .= "\n  FROM member_pay_history AS A ";
    $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
    $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
    $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
    $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND D.OPI_Date IS NULL AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751, 4290, 4301, 6820,6396) ";
    //$query .= "\n AND A.public_dvs IS NOT NULL  ";
    //$query .= "\n AND A.public_dvs = '세금계산서' AND A.public_state = '대기'  ";
    //$query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '대기') OR A.dvs = '입금조정')  ";

    //판매채널
    if ($this->blankParameterCheck($param ,"sell_site")) {
        $query .= "\n   AND  A.cpn_admin_seqno = ". $param["sell_site"];
    }

    //회원구분
    if ($this->blankParameterCheck($param ,"member_dvs")) {
        $member_dvs = substr($param["member_dvs"], 1, -1);
        if ($member_dvs == "개인") {

            $query .= "\n   AND  (A.member_dvs = '개인'";
            $query .= "\n    OR   A.member_dvs = '외국인')";

        } else {

            $query .= "\n   AND  (A.member_dvs = '기업'";
            $query .= "\n    OR   A.member_dvs = '기업개인')";
        }
    }

    //년도
    if ($this->blankParameterCheck($param ,"year")) {
        $query .= "\n   AND  A.pay_year = " . $param["year"];
    }

    //월
    if ($this->blankParameterCheck($param ,"mon")) {
        $query .= "\n   AND  A.pay_mon = " . $param["mon"];
    }

    if ($this->blankParameterCheck($param ,"member_seqno")) {
        $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
    }

    //업체명
    if ($this->blankParameterCheck($param ,"corp_name")) {

        $corp_name = substr($param["corp_name"], 1, -1);
        $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
    }

    if ($this->blankParameterCheck($param ,"dlvr_way")) {

        $dlvr_way = substr($param["dlvr_way"], 1, -1);
        if($dlvr_way == "01") {
            $query .= "\n   AND B.dlvr_add_info = '택배'";
        } else if($dlvr_way == "02") {
            $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
        } else if($dlvr_way == "05") {
            $query .= "\n   AND B.dlvr_add_info like '%방문%'";
        }
    }

    $s_num = substr($param["s_num"], 1, -1);
    $list_num = substr($param["list_num"], 1, -1);

    //$query .= "\n   ORDER BY public_admin_seqno DESC";

    if ($dvs == "SEQ") {
        $query .= "\n   GROUP BY B.member_seqno, A.public_dvs, A.public_state";
        $query .= "\n   ORDER BY B.sell_channel desc, B.member_name asc ";
    }

    return $conn->Execute($query);
}


    /**
     * @brief 매출계산서 현금영수증리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCashreceiptList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(DISTINCT B.member_seqno) as cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT A.deal_date, B.sell_channel,B.member_seqno, C.crn, C.corp_name, B.member_name, A.cont, A.dvs_detail, A.public_state, A.public_dvs, B.tel_num, B.cell_num, A.member_pay_history_seqno, A.order_num, A.dvs_detail, ";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price ,IFNULL(SUM(A.card_depo_price),0) AS card_depo_price";
            //$query .= "\n  A.pay_price ";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL)  ";
        $query .= "\n AND (A.public_dvs = '지출증빙' OR A.public_dvs = '소득공제') AND A.pay_price != 0  AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
 
        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.sell_channel  = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //회원
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //월
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  A.public_state = " . $param["state"];
        }

        //업체명
        if ($this->blankParameterCheck($param ,"corp_name")) {

            $corp_name = substr($param["corp_name"], 1, -1); 
            $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);


        if ($dvs == "SEQ") {
            $query .= "\n   GROUP BY A.member_seqno, A.dvs_detail ";
            $query .= "\n   ORDER BY A.public_state ASC";
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        //$conn->debug = 1;
        return $conn->Execute($query);
    }

    function selectCashreceiptList2($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(DISTINCT B.member_seqno) as cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT A.deal_date, B.sell_channel,B.member_seqno, C.crn, C.corp_name, B.member_name, A.cont, A.dvs_detail, A.public_state, A.public_dvs, B.tel_num, B.cell_num, A.member_pay_history_seqno, A.order_num, A.dvs_detail, ";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price, IFNULL(SUM(A.depo_price),0) AS depo_price ,IFNULL(SUM(A.card_depo_price),0) AS card_depo_price";
            //$query .= "\n  A.pay_price ";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL)  ";
        $query .= "\n AND (A.public_dvs = '지출증빙' OR A.public_dvs = '소득공제') AND A.pay_price != 0  AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
 
        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  B.sell_channel  = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //회원
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //월
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  A.public_state = " . $param["state"];
        }

        //업체명
        if ($this->blankParameterCheck($param ,"corp_name")) {

            $corp_name = substr($param["corp_name"], 1, -1); 
            $query .= "\n   AND  B.corp_name like '%" . $corp_name . "%'";
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);


        if ($dvs == "SEQ") {
            $query .= "\n   GROUP BY A.member_seqno, A.dvs_detail ";
            $query .= "\n   ORDER BY A.public_state ASC";
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        //$conn->debug = 1;
        //return $conn->Execute($query);
        echo $query;
    }

    function selectCashreceiptListBycase($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT A.deal_date, B.sell_channel,B.member_seqno, C.crn, C.corp_name, B.member_name, A.cont, A.dvs_detail, A.public_state, A.tax_price, A.public_dvs, B.tel_num, A.member_pay_history_seqno, A.order_num";
            $query .= "\n  ,IFNULL(SUM(A.pay_price),0) pay_price, IFNULL(SUM(A.card_pay_price),0) card_pay_price, IFNULL(SUM(A.adjust_price),0) adjust_price";
            //$query .= "\n  A.pay_price ";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL)  ";
        $query .= "\n AND A.pay_price != 0  AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
        $query .= "\n   AND  A.dvs_detail = " . $param["dvs_detail"];
        $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];


        if($param["kind"] == "'현금영수증'") {
            $query .= "\n   AND (A.public_dvs = '지출증빙' OR A.public_dvs = '소득공제') ";
            if ($this->blankParameterCheck($param ,"state")) {
                $query .= "\n   AND  A.public_state = " . $param["state"];
            }
        } else if($param["kind"] == "'세금계산서'") {
            $query .= "\n   AND (A.public_dvs = '세금계산서') ";
            if ($this->blankParameterCheck($param ,"state")) {
                $query .= "\n   AND  A.public_state = " . $param["state"];
            }
        } else if($param["kind"] == "'미발행'") {
            $query .= "\n   AND (A.public_dvs = '미발행') ";
        }

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = ". $param["sell_site"];
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"month")) {
            $query .= "\n   AND  A.pay_mon = " . $param["month"];
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }


        $query .= "\n   GROUP BY A.order_num";
        $query .= "\n   ORDER BY public_state ASC";

        return $conn->Execute($query);
    }


    function selectCardpayListBycase($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT A.deal_date, B.sell_channel,B.member_seqno, C.crn, C.corp_name, B.member_name, A.cont, A.dvs_detail, A.public_state, A.public_dvs, B.tel_num, A.member_pay_history_seqno, A.order_num, D.title ";
            $query .= "\n  ,IFNULL(SUM(A.pay_price),0) pay_price, IFNULL(SUM(A.card_pay_price),0) card_pay_price, IFNULL(SUM(A.adjust_price),0) adjust_price";
            //$query .= "\n  A.pay_price ";
        } else if ($dvs == "SUM") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL)  ";
        $query .= "\n AND A.card_pay_price != 0  AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
        //$query .= "\n   AND  D.pay_way = '카드' ";
        $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = ". $param["sell_site"];
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"month")) {
            $query .= "\n   AND  A.pay_mon = " . $param["month"];
        }

        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  A.public_state = " . $param["state"];
        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }


        $query .= "\n   GROUP BY A.order_num";
        $query .= "\n   ORDER BY public_state ASC";
        //$conn->debug = 1;
        return $conn->Execute($query);
    }


    /**
     * @brief 매출계산서 미발행(현금순매출)리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectUnissuedList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(DISTINCT A.member_seqno) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT B.sell_channel, A.deal_date, B.member_seqno, D.crn, D.corp_name, B.member_name, A.cont, A.dvs_detail, A.public_state, A.public_dvs, B.tel_num, B.cell_num, A.member_pay_history_seqno ";
            $query .= "\n  ,IFNULL(SUM(A.pay_price),0) pay_price, IFNULL(SUM(A.card_pay_price),0) card_pay_price, IFNULL(SUM(A.adjust_price),0) adjust_price";
        } else if ($dvs == "SUM" || $dvs == "SUM2") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  INNER JOIN order_common AS C ON A.order_num = C.order_num ";
        $query .= "\n  LEFT JOIN licensee_info AS D ON A.member_seqno = D.member_seqno ";
        $query .= "\n WHERE (C.order_state NOT IN ('1080','1120', '1180','1220') OR C.order_state IS NULL)  AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
        $query .= "\n   AND  A.public_dvs = '미발행' AND A.pay_price != 0 ";

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //검색어
        if ($this->blankParameterCheck($param ,"search")) {

            $search = substr($param["search"], 1, -1); 
            $search_dvs = substr($param["search_dvs"], 1, -1);

            if ($search_dvs == "") {

                $query .= "\n   AND  (B.corp_name like '%" . $search . "%'";
                $query .= "\n    OR   B.cashreceipt_num like '%" . $search . "%')";

            } else if ($search_dvs == "name") {

                $query .= "\n   AND  B.corp_name like '%" . $search . "%'";

            } else {

                $query .= "\n   AND  A.cashreceipt_num like '%" . $search . "%'";

            }

        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }


        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);


        if ($dvs == "SEQ") {
            $query .= "\n   GROUP BY A.member_seqno ";
            $query .= "\n ORDER BY B.sell_channel Desc ";
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }
 
    /**
     * @brief 세금계산서(발급완료) 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPublicCompleteList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(DISTINCT B.member_seqno) as cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT B.member_seqno, B.member_name, C.crn, C.corp_name, B.tel_num,  B.cell_num,";
            $query .= "\n  IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price, IFNULL(SUM(A.adjust_price),0) AS adjust_price ";
        } else if ($dvs == "SUM" || $dvs == "SUM2") {
            $query  = "\nSELECT IFNULL(SUM(A.pay_price),0) AS pay_price, IFNULL(SUM(A.card_pay_price),0) AS card_pay_price ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '에누리' then A.adjust_price  ELSE 0 END) enuri ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '매출' then A.adjust_price  ELSE 0 END) adjust_sales ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' then A.adjust_price  ELSE 0 END) adjust_deposit ";
            $query .= "\n  , SUM(CASE WHEN A.input_typ = '입금' THEN A.depo_price ELSE 0 END) adjust_etc ";
        }
        $query .= "\n  FROM member_pay_history AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  LEFT JOIN licensee_info AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  LEFT JOIN order_common AS D ON A.order_num = D.order_num ";
        $query .= "\n WHERE (D.order_state NOT IN ('1080','1120', '1180','1220') OR D.order_state IS NULL) AND A.member_seqno NOT IN (5044,6818,6842,6711, 6813, 6713,6714,4282,6840,6841,4284,6751) ";
        if ($dvs == "SUM") {
            $query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '완료') OR (A.public_dvs = '미발행' AND (A.dvs = '입금조정' OR A.dvs = '입금증가') AND (A.public_state = '완료'))) ";
        } else {
            $query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '완료') OR (A.public_dvs = '미발행' AND (A.dvs = '입금조정' OR A.dvs = '입금증가') AND (A.public_state = '완료'))) ";
            //$query .= "\n AND ((A.public_dvs = '세금계산서' AND A.public_state = '완료') OR (A.public_dvs = '미발행' AND D.order_state IS NULL)) ";
        }
        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .= "\n   AND  A.cpn_admin_seqno = ". $param["sell_site"];
        }

        //회원구분
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            if ($member_dvs == "개인") { 

                $query .= "\n   AND  (A.member_dvs = '개인'";
                $query .= "\n    OR   A.member_dvs = '외국인')";

            } else {

                $query .= "\n   AND  (A.member_dvs = '기업'";
                $query .= "\n    OR   A.member_dvs = '기업개인')";
            }
        }

        //년도
        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  A.pay_year = " . $param["year"];
        }

        //월
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  A.pay_mon = " . $param["mon"];
        }

        //월
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  A.member_seqno = " . $param["member_seqno"];
        }

        //발급 유형
        if ($this->blankParameterCheck($param ,"tab_public")) {
            $query .= "\n   AND  B.tab_public = " . $param["tab_public"];
        }

        //검색어
        if ($this->blankParameterCheck($param ,"search")) {

            $search = substr($param["search"], 1, -1); 
            $search_dvs = substr($param["search_dvs"], 1, -1);

            if ($search_dvs == "") {

                $query .= "\n   AND  (B.corp_name like '%" . $search . "%'";
                $query .= "\n    OR   B.crn like '%" . $search . "%'";
                $query .= "\n    OR   B.repre_name like '%" . $search . "%')";

            } else if ($search_dvs == "name") {

                $query .= "\n   AND  B.corp_name like '%" . $search . "%'";

            } else if ($search_dvs == "num"){

                $query .= "\n   AND  B.crn like '%" . $search . "%'";
            } else {

                $query .= "\n   AND  B.repre_name like '%" . $search . "%'";
            }

        }

        if ($this->blankParameterCheck($param ,"dlvr_way")) {

            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            if($dlvr_way == "01") {
                $query .= "\n   AND B.dlvr_add_info = '택배'";
            } else if($dlvr_way == "02") {
                $query .= "\n   AND (B.dlvr_add_info like 'A%' OR B.dlvr_add_info like 'B%')" ;
            } else if($dlvr_way == "05") {
                $query .= "\n   AND B.dlvr_add_info like '%방문%'";
            }
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\n   GROUP BY B.member_seqno";
        }

        if ($dvs == "SEQ") { 
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    function updatePublicState($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history ";
        $query .= "\n    SET public_state = " . $param["new_state"];
        $query .= "\n    WHERE  member_seqno = ". $param["member_seqno"];
        $query .= "\n    AND  (public_dvs = '세금계산서' OR dvs = '입금조정' OR dvs = '입금증가') ";
        if($param["now_state"] == "'대기'") {
            $query .= "AND (public_state = '' OR public_state = " . $param["now_state"] . ")";
        } else {
            $query .= "AND public_state = " . $param["now_state"];
        }

        $query .= "\n    AND  pay_year = " . $param["year"];
        $query .= "\n    AND  pay_mon = " . $param["mon"];

        if ($this->blankParameterCheck($param ,"public_state")) {
            $query .= "\n    AND  public_state = " . $param["public_state"];
        }

        return $conn->Execute($query);
    }

    function selectTaxPrice($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);        

         $query2  = "\n    SELECT COUNT(*) AS cnt FROM tax_change_price";
         $query2 .= "\n    WHERE member_seqno = ". $param["member_seqno"];
         $query2 .= "\n    AND  member_year = " . $param["year"];
         $query2 .= "\n    AND  member_mon = " . $param["mon"];

         $result = $conn->Execute($query2);

        return $result;

    }
    function selectTaxChkPrice($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);        

         $query2  = "\n    SELECT change_price FROM tax_change_price";
         $query2 .= "\n    WHERE member_seqno = ". $param["member_seqno"];
         $query2 .= "\n    AND  member_year = " . $param["year"];
         $query2 .= "\n    AND  member_mon = " . $param["mon"];

         $result = $conn->Execute($query2);

        return $result;

    }

    function updateTaxPrice($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);        

        $query  = "\n    UPDATE tax_change_price ";
        $query .= "\n    SET change_price = " . $param["new_price"];
        $query .= "\n    WHERE  member_seqno = ". $param["member_seqno"];
        $query .= "\n    AND  member_year = " . $param["year"];
        $query .= "\n    AND  member_mon = " . $param["mon"];

        $result = $conn->Execute($query);

        return $result;

    }

    function insertTaxPrice($conn, $param) {
        if (!$this->connectionCheck($conn)) return false;
        $param = $this->parameterArrayEscape($conn, $param);
       

        $query  = "\n    INSERT INTO tax_change_price ";
        $query  .= "\n    (member_seqno, change_price, member_year,member_mon)";
        $query  .= "\n    VALUES ";
        $query  .= "\n    (%s, %s, %s, %s) ";

        $query  = sprintf($query
            ,$param["member_seqno"]
            ,$param["new_price"]
            ,$param["year"]
            ,$param["mon"]);
        return $conn->Execute($query);
    }

    function updatePublicStateNew($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history AS A";
        $query .= "\n    LEFT JOIN order_common AS B ON A.order_num = B.order_num ";
        $query .= "\n    SET A.public_dvs = " . $param["new_dvs"];
        if ($this->blankParameterCheck($param ,"public_state")) {
            $query .= "\n    , A.public_state = " . $param["public_state"];
        }
        if ($this->blankParameterCheck($param ,"pay_way")) {
            $query .= "\n    , B.pay_way = " . $param["pay_way"];
        }
        $query .= "\n    WHERE  A.member_seqno = ". $param["member_seqno"];
        $query .= "\n    AND  A.pay_year = " . $param["year"];
        $query .= "\n    AND  A.pay_mon = " . $param["mon"];
        $query .= "\n    AND  (A.public_dvs = " . $param["public_dvs"] . " OR A.dvs = '입금증가' OR A.dvs = '입금조정')";
        if ($this->blankParameterCheck($param ,"dvs_detail")) {
            $query .= "\n    AND A.dvs_detail = " . $param["dvs_detail"];
        }

        $conn->debug = 1;
        return $conn->Execute($query);
    }

    function updatePublicStateCase($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history ";
        $query .= "\n    SET public_dvs = " . $param["new_dvs"];
        $query .= "\n    WHERE  order_num = " . $param["order_num"];


        return $conn->Execute($query);
    }

    function updatePublicStateNewIssue($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history ";
        $query .= "\n    SET public_state = " . $param["state"];
        $query .= "\n    WHERE  1 = 1 ";
        //$query .= "\n    AND  public_dvs IN ('소득공제','지출증빙')";

        if($param["kind"] == "'현금영수증'") {
            $query .= "\n   AND (public_dvs = '지출증빙' OR public_dvs = '소득공제') ";
        } else if($param["kind"] == "'세금계산서'") {
            $query .= "\n   AND (public_dvs = '세금계산서') ";
        }

        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .= "\n   AND  member_seqno = " . $param["member_seqno"];
        }

        if ($this->blankParameterCheck($param ,"year")) {
            $query .= "\n   AND  pay_year = " . $param["year"];
        }
        if ($this->blankParameterCheck($param ,"mon")) {
            $query .= "\n   AND  pay_mon = " . $param["mon"];
        }
        if ($this->blankParameterCheck($param ,"order_num")) {
            $query .= "\n   AND  order_num = " . $param["order_num"];
        }
        if ($this->blankParameterCheck($param ,"detail_dvs")) {
            $query .= "\n   AND  dvs_detail = " . $param["detail_dvs"];
        }

        return $conn->Execute($query);
    }
}
?>
