<?
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . '/com/nexmotion/html/MakeCommonHtml.inc');

// 쿼리캐시 경로
$ADODB_CACHE_DIR = "/home/sitemgr/query_cache";

class CommonDAO {
    /**
     * @brief 배열값을 IN 조건 등에 들어갈 수 있도록 문자열로 변경
     *
     * @param $conn  = DB Connection
     * @param $param = 배열값
     *
     * @return 변환 된 배열
     */
    function arr2paramStr($conn, $param) {
        if (empty($param) === true || count($param) === 0) {
            return '';
        }

        $ret = '';

        foreach ($param as $val) {
            if (empty($val) === true) {
                continue;
            }

            $ret .= $this->parameterEscape($conn, $val) . ',';
        }

        $ret = substr($ret, 0, -1);

        return is_bool($ret) ? '' : $ret;
    }

    /**
     * @brief 캐쉬를 삭제하는 함수
     * @param $conn DB Connection
     */
    function cacheFlush($conn) {
        $conn->CacheFlush();
    }

   /**
     * @brief NULL 이거나 공백값('')이 아닌 파라미터만 체크
     * @param $param 임의의 배열 인자
     * @param $key 임의의 배열 인자의 키
     * @return boolean
     */
    function blankParameterCheck($param, $key) {
        // 파라미터가 빈 값이 아닐경우
        if ($param !== ""
                && isset($param[$key])
                && $param[$key] !== ""
                && $param[$key] !== ''
                && $param[$key] !== "''"
                && $param[$key] !== "NULL"
                && $param[$key] !== "null") {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief SQL 인젝션 방지용
     *
     * @param $conn  = DB Connection
     * @param $param = 검색조건
     *
     * @return 변환 된 인자
     */
    function parameterEscape($conn, $param) {
        $param = @htmlspecialchars($param, ENT_QUOTES, "UTF-8", false);
        $ret = $conn->qstr($param, get_magic_quotes_gpc());
        return $ret;
    }

    /**
     * @brief SQL 인젝션 방지용, 배열
     *
     * @detail $except_arr 배열은 $except["제외할 필드명"] = true
     * 형식으로 입력받는다.
     *
     * @param $conn       = DB Connection
     * @param $param      = 검색조건 배열
     * @param $except_arr = 이스케이프 제외할 필드명
     *
     * @return 변환 된 배열
     */
    function parameterArrayEscape($conn, $param, $except_arr = null) {
        if (!is_array($param)) return false;

        foreach ($param as $key => $val) {
            if ($except_arr[$key] === true) {
                continue;
            }

            if (is_array($val)) {
                $val = $this->parameterArrayEscape($conn, $val, $except_arr);
            } else {
                $val = $this->parameterEscape($conn, $val);
            }

            $param[$key] = $val;
        }

        return $param;
    }

    /** 
     * @brief 커넥션 검사
     * @param $conn DB Connection
     * @return boolean
     */ 
    function connectionCheck($conn) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        return true;
    }

    /**
     * @brief 데이터 검색 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["col"] = "컬럼명"<br>
     *        $param["where"]["컬럼명"] = "조건" (다중)<br>
     *        $param["not"]["컬럼명"] = "조건" (다중)<br>
     *        $param["order"] = "order by 조건"<br>
     *        $param["group"] = "group by 조건"<br>
     *        $param["cache"] = "1" 캐쉬 생성<br>
     *        $param["limit"]["start"] = 리미트 시작값<br>
     *        $param["limit"]["end"] =  리미트 종료값<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function selectData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n SELECT " . $param["col"] . " FROM " . $param["table"];

        $i = 0;
        $col = "";
        $value = "";

        if ($param["where"]) {

            foreach ($param["where"] as $key => $val) {

                $inchr = $conn->qstr($val,get_magic_quotes_gpc());

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . "=" . $inchr;
                 } else {
                        $value  .= "\n   AND " . $key . "=" . $inchr;
                 }
                $i++;
            }
        }

        //임시로 만듬
        if ($param["not"]) {
            foreach ($param["not"] as $key => $val) {

                $inchr = $conn->qstr($val,get_magic_quotes_gpc());
                $value  .= "\n AND NOT " . $key . "=" . $inchr;
                $i++;
            }
        }

        //like search
        if ($param["like"]) {
            foreach ($param["like"] as $key => $val) {

                $inchr = substr($conn->qstr($val,get_magic_quotes_gpc()),1, -1);

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . " LIKE '%" . $inchr . "%'";
                 } else {
                        $value  .= "\n   AND " . $key . " LIKE '%" . $inchr . "%'";
                 }
                $i++;
            }
        }

        //back like search
        if ($param["blike"]) {
            foreach ($param["blike"] as $key => $val) {

                $inchr = substr($conn->qstr($val,get_magic_quotes_gpc()),1, -1);

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . " LIKE '" . $inchr . "%'";
                 } else {
                        $value  .= "\n   AND " . $key . " LIKE '" . $inchr . "%'";
                 }
                $i++;
            }
        }

        $query .= $value;

        if ($param["group"]) {
            $query .= "\n GROUP BY " . $param["group"];
        }

        if ($param["order"]) {
            $query .= "\n ORDER BY " . $param["order"];
        }

        if ($param["limit"]) {

            $query .= "\n LIMIT " . $param["limit"]["start"] . ",";
            $query .= $param["limit"]["end"];
        }

        //Query Cache
        if ($param["cache"] == 1) {
            $rs = $conn->CacheExecute(1800, $query);
        } else {
            $rs = $conn->Execute($query);
        }

        return $rs;
    }

    /**
     * @brief COUNT 데이터 검색 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["where"]["컬럼명"] = "조건" (다중)<br>
     *        $param["cache"] = "1" 캐쉬 생성<br>
     *        $param["limit"]["start"] = 리미트 시작값<br>
     *        $param["limit"]["end"] =  리미트 종료값<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function countData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n SELECT count(*) cnt  FROM " . $param["table"];

        $i = 0;
        $col = "";
        $value = "";

        if ($param["where"]) {

            foreach ($param["where"] as $key => $val) {

                $inchr = $conn->qstr($val,get_magic_quotes_gpc());

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . "=" . $inchr;
                 } else {
                        $value  .= "\n   AND " . $key . "=" . $inchr;
                 }
                $i++;
            }
        }

        if ($param["like"]) {

            foreach ($param["like"] as $key => $val) {

                $inchr = $conn->qstr($val,get_magic_quotes_gpc());

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . " LIKE " . $inchr;
                 } else {
                        $value  .= "\n   AND " . $key . " LIKE " . $inchr;
                 }
                $i++;
            }
        }

        if ($param["group"]) {
            $query .= "\n GROUP BY " . $param["group"];
        }

       if ($param["limit"]) {
            $query .= "\n LIMIT " . $param["limit"]["start"] . ",";
            $query .= $param["limit"]["end"];
        }

        $query .= $value;

        $rs = $conn->Execute($query);
        return $rs;

    }

    /**
     * @brief DISTINCT 데이터 검색 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["col"] = "컬럼명"<br>
     *        $param["where"]["컬럼명"] = "조건" (다중)<br>
     *        $param["order"] = "ORDER BY 컬럼"<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function distinctData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n SELECT DISTINCT " . $param["col"] . " FROM " . $param["table"];
        $i = 0;
        $value = "";

        if ($param["where"]) {

            foreach ($param["where"] as $key => $val) {

                $inchr = $conn->qstr($val, get_magic_quotes_gpc());

                if ($i == 0) {
                        $value  .= "\n WHERE " . $key . "=" . $inchr;
                 } else {
                        $value  .= "\n   AND " . $key . "=" . $inchr;
                 }
                $i++;
            }
        }

        $query .= $value;

        if ($param["order"]) {
            $query .= "\n ORDER BY " . $param["order"];
        }

        //Query Cache
        if ($param["cache"] == 1) {
            $rs = $conn->CacheExecute(1800, $query);
        } else {
            $rs = $conn->Execute($query);
        }

        return $rs;
    }

    /**
     * @brief 전체 데이터 삭제 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["prk"] = "primary key colulm"<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function deleteAllData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query  = "\n DELETE ";
        $query .= "\n   FROM " . $param["table"];
        $query .= "\n  WHERE " . $param["prk"] . " >= 0";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 삭제에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 데이터 삭제 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["prk"] = "primary key column"<br>
     *        $param["prkVal"] = "primary data" <br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function deleteData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query  = "\n DELETE ";
        $query .= "\n   FROM " . $param["table"];
        $query .= "\n  WHERE " . $param["prk"];
        $query .= "\n       =" . $conn->qstr($param["prkVal"], get_magic_quotes_gpc());

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 삭제에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 다중 데이터 삭제 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["prk"] = "primary key colulm" <br>
     *        $param["prkVal"] = "primary data"  ex) 1,2,3,4 <br>
     *        $param["not"] = "제외 검색"  ex) Y<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function deleteMultiData($conn, $param) {

        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query  = "\n DELETE ";
        $query .= "\n   FROM " . $param["table"];
        $query .= "\n  WHERE " . $param["prk"];
        $query .= "\n     IN (";

        $prkValCount = count($param["prkVal"]);
        for ($i = 0; $i < $prkValCount; $i++) {
            $val = $conn->qstr($param["prkVal"][$i], get_magic_quotes_gpc());
            $query .= $val;

            if ($i !== $prkValCount - 1) {
                $query .= ",";
            }
        }
        $query .= ")";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 삭제에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 데이터 삽입/수정 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["col"]["컬럼명"] = "데이터" (다중)<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function replaceData($conn, $param) {

        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n INSERT INTO " . $param["table"] . "(";

        $i = 0;
        $col = "";
        $value = "";

        foreach ($param["col"] as $key => $val) {

            $inchr = $conn->qstr($val,get_magic_quotes_gpc());
            if ($i == 0) {
                $col  .= "\n " . $key;
                $value  .= "\n " . $inchr;
            } else {
                $col  .= "\n ," . $key;
                $value  .= "\n ," . $inchr;
            }

            $i++;
        }

        $query .= $col;
        $query .= "\n ) VALUES (";
        $query .= $value;
        $query .= "\n )";
        $query .= "\n ON DUPLICATE KEY UPDATE";

        $i = 0;
        $col = "";
        $value = "";

        reset($param["col"]);

        foreach ($param["col"] as $key => $val) {
            $inchr = $conn->qstr($val,get_magic_quotes_gpc());

            if ($i == 0) {
                $value  .= "\n " . $key . "=" . $inchr;
            } else {
                $value  .= "\n ," . $key . "=" . $inchr;
            }

            $i++;
        }
        $query .= $value;

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 입력에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 데이터 삽입 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["col"]["컬럼명"] = "데이터" (다중)<br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function insertData($conn, $param) {

        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n INSERT INTO " . $param["table"] . "(";

        $i = 0;
        $col = "";
        $value = "";

        foreach ($param["col"] as $key => $val) {

            $inchr = $conn->qstr($val,get_magic_quotes_gpc());

            if ($i == 0) {
                $col  .= "\n " . $key;
                $value  .= "\n " . $inchr;
            } else {
                $col  .= "\n ," . $key;
                $value  .= "\n ," . $inchr;
            }

            $i++;
        }

        $query .= $col;
        $query .= "\n ) VALUES (";
        $query .= $value;
        $query .= "\n )";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 입력에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 데이터 수정 쿼리 함수 (공통)<br>
     *        param 배열 설명<br>
     *        $param : $param["table"] = "테이블명"<br>
     *        $param["col"]["컬럼명"] = "수정데이터" (다중)<br>
     *        $param["prk"] = "primary key colulm"<br>
     *        $param["prkVal"] = "primary data" <br>
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function updateData($conn, $param) {

        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $query = "\n UPDATE " . $param["table"]  . " set";

        $i = 0;
        $col = "";
        $value = "";

        foreach ($param["col"] as $key => $val) {

         //   $inchr = $val;
            $inchr = $conn->qstr($val,get_magic_quotes_gpc());

            if ($i == 0) {
                $value  .= "\n " . $key . "=" . $inchr;
            } else {
                $value  .= "\n ," . $key . "=" . $inchr;
            }

            $i++;
        }

        $query .= $value;
        $query .= " WHERE " . $param["prk"] . "=" . $conn->qstr($param["prkVal"], get_magic_quotes_gpc());

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 다중 데이터 수정 쿼리 함수 (공통) <br>
     *        param 배열 설명 <br>
     *        $param : <br>
     *        $param["table"] = "테이블명"<br>
     *        $param["col"]["컬럼명"] = "수정데이터" (다중)<br>
     *        $param["prk"] = "primary key colulm"<br>
     *        $param["prkVal"] = "primary data"  ex) 1,2,3,4
     * @param $conn DB Connection
     * @param $param 파라미터 인자 배열
     * @return boolean
     */
    function updateMultiData($conn, $param) {
        if (!$conn) {
            echo "master connection failed\n";
            return false;
        }

        $prkArr = str_replace(" ", "", $param["prkVal"]);
        $prkArr = str_replace("'", "", $prkArr);
        $prkArr = explode(",", $prkArr);

        $parkVal = "";

        for ($i = 0; $i < count($prkArr); $i++) {
            $prkVal .= $conn->qstr($prkArr[$i], get_magic_quotes_gpc()) . ",";
        }
        $prkVal = substr($prkVal, 0, -1);

        $query = "\n UPDATE " . $param["table"]  . " set";

        $i = 0;
        $col = "";
        $value = "";

        foreach ($param["col"] as $key => $val) {

            $inchr = $conn->qstr($val,get_magic_quotes_gpc());

            if ($i == 0) {
                $value  .= "\n " . $key . "=" . $inchr;
            } else {
                $value  .= "\n ," . $key . "=" . $inchr;
            }

            $i++;
        }

        $query .= $value;
        $query .= " WHERE " . $param["prk"] . " in(";
        $query .= $prkVal . ")";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $errorMessage = "데이터 수정에 실패 하였습니다.";
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 리스트 쿼리 row 수 반환
     *
     * @detail https://blog.asamaru.net/2015/09/11/using-sql-calc-found-rows-and-found-rows-with-mysql/
     *
     * @param $conn = connection identifier
     *
     * @return 카테고리명
     */
    function selectFoundRows($conn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT FOUND_ROWS() AS count";

        $rs = $conn->Execute($query);

        return $rs->fields["count"];
    }

    /*
     * 은행이름 list Select 
     * $conn : DB Connection
     * return : resultSet 
     */ 
    function selectBankNameList($conn, $param) {

        if (!$this->connectionCheck($conn)) return false; 
        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n  SELECT  DISTINCT bank_name";
        $query .= "\n    FROM  virt_ba_admin";
        //$query .= "\n    FROM  virt_ba_admin";
        //$query .= "\n   WHERE  cpn_admin_seqno = " . $param["cpn_admin_seqno"];

        $result = $conn->Execute($query);

        return $result;
    }

    /**
     * @brief 선택한 회원의 일매출 통계 수정(매출 컬럼)
     *
     * @writer montvert(2017.07.25)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function modiSalesDaySalesStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO day_sales_stats (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,sales_price";
        $query .= "\n       ,net_sales_price";
        $query .= "\n       ,input_date";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";
        $query .= "\n  ON DUPLICATE KEY UPDATE";
        $query .= "\n  sales_price     = IFNULL(day_sales_stats.sales_price, 0) + %s";
        $query .= "\n ,net_sales_price = IFNULL(day_sales_stats.net_sales_price, 0) + %s";
        $query  = sprintf($query, $param["member_seqno"]
                                , $param["modi_price"] 
                                , $param["modi_price"]
                                , $param["deal_date"]
                                , $param["modi_price"]
                                , $param["modi_price"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 일매출 통계 수정(입금 컬럼)
     *
     * @writer montvert(2017.07.25)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function modiDepoDaySalesStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO day_sales_stats (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,depo_price";
        $query .= "\n       ,input_date";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";
        $query .= "\n  ON DUPLICATE KEY UPDATE";
        $query .= "\n  depo_price = IFNULL(day_sales_stats.depo_price, 0) + %s";
        $query  = sprintf($query, $param["member_seqno"]
                                , $param["modi_price"] 
                                , $param["deal_date"]
                                , $param["modi_price"]);

        return $conn->Execute($query);

    }

    /**
     * @brief 선택한 회원의 일매출 통계 수정(에누리, 순매출액 컬럼)
     *
     * @writer montvert(2017.07.25)
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function modiSaleDaySalesStats($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO day_sales_stats (";
        $query .= "\n        member_seqno";
        $query .= "\n       ,input_date";
        $query .= "\n       ,sale_price";
        $query .= "\n       ,net_sales_price";
        $query .= "\n ) VALUES (";
        $query .= "\n        %s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s ";
        $query .= "\n       ,%s)";
        $query .= "\n  ON DUPLICATE KEY UPDATE";
        $query .= "\n   depo_price      = %s";
        $query .= "\n  ,net_sales_price = %s";
        $query  = sprintf($query, $param["member_seqno"]
                                , $param["input_date"]
                                , $param["sale_price"] 
                                , $param["net_sales_price"] 
                                , $param["sale_price"] 
                                , $param["net_sales_price"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 회원 등급과 등급명 검색
     *
     * @param $conn  = connection identifier
     *
     * @return 검색결과
     */
    function selectGradeSalePolicy($conn) {
        if (!$this->connectionCheck($conn)) {
           return false;
        }
        
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.front_grade_name";
        $query .= "\n        ,A.erp_grade_name";
        $query .= "\n        ,A.grade";
        $query .= "\n   FROM  member_grade_policy AS A";

        $rs = $conn->Execute($query);

        return $rs;
    }
}
?>
