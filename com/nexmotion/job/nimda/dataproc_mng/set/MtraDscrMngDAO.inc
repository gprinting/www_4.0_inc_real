<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/com/nexmotion/html/nimda/dataproc_mng/set/MtraDscrMngHTML.inc");

class MtraDscrMngDAO extends NimdaCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /*
     * 종이설명 list Select 
     * $conn : db connection
     * $param["paper_name"] : 종이명
     * return : resultset 
     */ 
    function selectPaperDscrList($conn, $param) {
        if (!$this->connectioncheck($conn)) return false; 

        $param = $this->parameterarrayescape($conn, $param);

        $query  = "\n    SELECT   SQL_CALC_FOUND_ROWS";
        $query .= "\n             top";
        $query .= "\n            ,name";
        $query .= "\n            ,dvs";
        $query .= "\n            ,color";
        $query .= "\n            ,basisweight";
        $query .= "\n            ,dscr";
        $query .= "\n            ,purp";
        $query .= "\n            ,paper_sense";
        $query .= "\n            ,able_after";
        $query .= "\n            ,warn";
        $query .= "\n            ,paper_dscr_seqno";
        $query .= "\n      FROM   paper_dscr";

        //종이명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $query .= "\n  ORDER BY paper_dscr_seqno";

        //limit 조건
        if ($this->blankParameterCheck($param ,"start") && $this->blankParameterCheck($param ,"end")) {
 
            $param["start"] = substr($param["start"], 1, -1);
            $param["end"] = substr($param["end"], 1, -1); 

            $query .= "\n LIMIT " . $param["start"] . ",";
            $query .= $param["end"]; 
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 종이설명 list Count
     * $conn : db connection
     * $param["paper_name"] : 종이명
     * return : resultset 
     */ 
    function countPaperDscrList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterarrayescape($conn, $param);
        $query  = "\n    SELECT   count(*) cnt";
        $query .= "\n      FROM   paper_dscr";

        //종이명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $result = $conn->Execute($query);

        return $result;
    }

    function selectDirectDlvrList($conn) {
        $query  = "\n    SELECT   direct_dlvr_info_seqno, vehi_num ";
        $query .= "\n      FROM   direct_dlvr_info ";

        $result = $conn->Execute($query);

        return makeOptionHtml($result, "direct_dlvr_info_seqno", "vehi_num", null, "N");;
    }

    function selectPayPrice($conn, $param) {
        $query  = "\n    SELECT   SUM(pay_price) as pay_price ";
        $query .= "\n      FROM   order_common ";
        $query .= "\n      WHERE   order_regi_date LIKE '". $param["order_regi_date"] ."%' ";
        $query .= "\n      AND member_seqno =  " . $param["member_seqno"];
        //$query .= "\n      AND member_seqno =  " . $param["member_seqno"];

        $result = $conn->Execute($query);

        return $result->fields["pay_price"];
    }

    function selectDlvrList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $preset_cate = $param["preset_cate"];
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\n SELECT C.member_name, C.addr, C.addr_detail, order_regi_date, order_state, A.member_seqno, D.erp_state_name ";
        }
        $query .= "\n  FROM order_common AS A ";
        $query .= "\n  INNER JOIN order_dlvr AS B ON A.order_common_seqno = B.order_common_seqno AND tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN member AS C ON A.member_seqno = C.member_seqno ";
        $query .= "\n  INNER JOIN state_admin AS D ON D.state_code = A.order_state ";
        $query .= "\n  WHERE B.dlvr_way = '02' AND A.order_state IN ('3220','3320','3380') ";


        //if ($this->blankParameterCheck($param ,"from")) {
        //    $val = substr($param["date_cnd"], 1, -1);
        //    $query .= "\n   AND  A.order_regi_date > " . $param["from"];
        //}
        //if ($this->blankParameterCheck($param ,"to")) {
        //    $val = substr($param["date_cnd"], 1, -1);
        //    $query .= "\n   AND  A.order_regi_date <= " . $param["to"];
        //}
        if ($this->blankParameterCheck($param ,"typset_num")) {
            $val = substr($param["search_txt"], 1, -1);
            $query .= "\n   AND  A.name LIKE '% " . $val . "%' ";
        }

        $query .= "\nORDER BY A.order_common_seqno DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    function selectDirectDlvrDetail($conn, $param) {
        if (!$this->connectioncheck($conn)) return false;
        $param = $this->parameterarrayescape($conn, $param);

        $query  = "\n    SELECT   * ";
        $query .= "\n      FROM   direct_dlvr_info ";
        $query .= "\n      WHERE   direct_dlvr_info_seqno = %s ";

        $query = sprintf($query, $param['direct_dlvr_info_seqno']);
        return $conn->Execute($query);
    }

    function updateDirectDlvrDetail($conn, $param) {
        if (!$this->connectioncheck($conn)) return false;
        $param = $this->parameterarrayescape($conn, $param);

        $query  = "\n    UPDATE direct_dlvr_info ";
        $query .= "\n    SET mng = %s ";
        $query .= "\n    ,vehi_num = %s ";
        $query .= "\n    ,car_number = %s ";
        $query .= "\n    ,is_using = %s ";
        $query .= "\n    ,dlvr_area = %s ";
        $query .= "\n    ,method = %s ";
        $query .= "\n    ,cost_by_case = %s ";
        $query .= "\n      WHERE   direct_dlvr_info_seqno = %s ";

        $query = sprintf($query,
            $param['mng'],
            $param['vehi_num'],
            $param['car_number'],
            $param['is_using'],
            $param['dlvr_area'],
            $param['method'],
            $param['cost_by_case'],
            $param['direct_dlvr_info_seqno']);
        return $conn->Execute($query);
    }

    function inputDirectDlvr($conn, $param) {
        if (!$this->connectioncheck($conn)) return false;
        $param = $this->parameterarrayescape($conn, $param);

        $query  = "\n    INSERT INTO direct_dlvr_info ";
        $query .= "\n   (vehi_num, mng, dlvr_area, car_number, is_using) VALUES ";
        $query .= "\n  (%s, %s, %s, %s, 'Y')";

        $query = sprintf($query,
            $param['vehi_num'],
            $param['mng'],
            $param['dlvr_area'],
            $param['car_number']);
        return $conn->Execute($query);
    }

    /*
     * 후공정설명 list Select 
     * $conn : db connection
     * $param["after_name"] : 후공정명
     * return : resultset 
     */ 
    function selectAfterDscrList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterarrayescape($conn, $param);
        $query  = "\n    SELECT   name";
        $query .= "\n            ,dscr";
        $query .= "\n            ,after_dscr_seqno";
        $query .= "\n      FROM   after_dscr";

        //후공정명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $query .= "\n  ORDER BY after_dscr_seqno";

        //limit 조건
        if ($this->blankParameterCheck($param ,"start") && $this->blankParameterCheck($param ,"end")) {
 
            $param["start"] = substr($param["start"], 1, -1);
            $param["end"] = substr($param["end"], 1, -1); 

            $query .= "\n LIMIT " . $param["start"] . ",";
            $query .= $param["end"]; 
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 후공정설명 list Count
     * $conn : db connection
     * $param["after_name"] : 후공정명
     * return : resultset 
     */ 
    function countAfterDscrList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterarrayescape($conn, $param);
        $query  = "\n    SELECT   count(*) cnt";
        $query .= "\n      FROM   after_dscr";

        //후공정명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 옵션설명 list Select 
     * $conn : db connection
     * $param["opt_name"] : 옵션명
     * return : resultset 
     */ 
    function selectOptDscrList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterarrayescape($conn, $param);
        $query  = "\n    SELECT   name";
        $query .= "\n            ,dscr";
        $query .= "\n            ,opt_dscr_seqno";
        $query .= "\n      FROM   opt_dscr";

        //옵션명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $query .= "\n  ORDER BY opt_dscr_seqno";

        //limit 조건
        if ($this->blankParameterCheck($param ,"start") && $this->blankParameterCheck($param ,"end")) {
 
            $param["start"] = substr($param["start"], 1, -1);
            $param["end"] = substr($param["end"], 1, -1); 

            $query .= "\n LIMIT " . $param["start"] . ",";
            $query .= $param["end"]; 
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /*
     * 옵션설명 list Count
     * $conn : db connection
     * $param["opt_name"] : 종이명
     * return : resultset 
     */ 
    function countOptDscrList($conn, $param) {

        if (!$this->connectioncheck($conn)) return false; 
        $param = $this->parameterarrayescape($conn, $param);
        $query  = "\n    SELECT   count(*) cnt";
        $query .= "\n      FROM   opt_dscr";

        //옵션명 검색 있을때
        if ($this->blankParameterCheck($param ,"search_str")) {

            $search_str = substr($param["search_str"], 1, -1); 

            $query .= "\n      WHERE  name LIKE '%" . $search_str . "%' ";
        }

        $result = $conn->Execute($query);

        return $result;
    }






}
?>
