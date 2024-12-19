<?

include_once(INC_PATH . "/com/nexmotion/job/front/common/FrontCommonDAO.inc");
include_once(INC_PATH . '/define/front/product_default_sel.inc');
include_once(INC_PATH . '/common_define/prdt_default_info.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/product/ProductCommonHtml.inc');

class ProductCommonDAO extends FrontCommonDAO {
    function __construct() {
    }

    /**
     * @brief 카테고리 사진 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["seq"] = 사진 순서
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCatePhoto($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "file_path, save_file_name";
        $temp["table"] = "cate_photo";
        $temp["where"]["cate_sortcode"] = $param["cate_sortcode"];
        if ($this->blankParameterCheck($param, "link")) {
            $temp["where"]["link"] = $param["link"];
        }

        $temp["order"] = "seq ASC";

        return $this->selectData($conn, $temp);
    }

    function selectSizeName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT name FROM prdt_stan AS A ";
        $query .= "\n    INNER JOIN cate_stan AS B ON A.prdt_stan_seqno = B.prdt_stan_seqno ";
        $query .= "\n    WHERE B.mpcode = %s ";

        $query  = sprintf($query, $param["stan_mpcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields["name"];
    }

    /**
     * @brief 카테고리 배너 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 검색조건 파라미터
     * @param $is_info       = 정보생성인지 이미지 출력인지 구분
     *
     * @return 검색결과
     */
    function selectCateBanner($conn, $cate_sortcode, $is_info = true) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $col = "";
        if ($is_info === true) {
            $col = "file_path, save_file_name, url_addr, target_yn";
        } else {
            $col = "file_path, save_file_name";
        }

        $temp = array();
        $temp["col"]   = $col;
        $temp["table"] = "cate_banner";
        $temp["where"]["cate_sortcode"] = $cate_sortcode;

        return $this->selectData($conn, $temp);
    }

    /**
     * @brief 카테고리 종이분류 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     * @param &$price_info_arr = 가격검색용 정보저장 배열
     *
     * @return option html
     */
    function selectCatePaperSortHtml($conn, $param, &$price_info_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $default = $param["default"];
        if (empty($default) === true) {
            $default = true;
        }

        $cate_sortcode = $param["cate_sortcode"];

        $temp = array();
        $temp["col"]   .= "TRIM(sort) AS sort";
        $temp["table"]  = "cate_paper";
        $temp["where"]["cate_sortcode"] = $cate_sortcode;
        $temp["where"]["use_yn"] = "Y";
        $temp["order"]  = "sort";
        $rs = $this->distinctData($conn, $temp);
        return makeCatePaperSortOption($rs, $default, $price_info_arr);
    }

    /**
     * @brief 카테고리 종이명 html 생성 
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     * @param &$price_info_arr = 가격검색용 정보저장 배열
     *
     * @return option html
     */
    function selectCatePaperNameHtml($conn, $param, &$price_info_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $default = $param["default"];
        if (empty($default) === true) {
            $default = true;
        }
        $rs = $this->selectCatePaperName($conn, $param);
        return makeCatePaperNameOption($rs, $default, $price_info_arr);
    }

    /**
     * @brief 카테고리 종이명 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     *
     * @return option html
     */
    function selectCatePaperName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "  SELECT DISTINCT TRIM(name) AS name, cate_sortcode ";
        $query .= " FROM cate_paper ";
        $query .= " WHERE cate_sortcode = " . $param["cate_sortcode"];
        $query .= " AND use_yn = 'Y' ";

        if ($this->blankParameterCheck($param, "sort")) {
            $query .= " AND sort = '" . $param["sort"] . "'";
        }
        if ($this->blankParameterCheck($param, "link")) {
            $query .= " AND link = '" . $param["link"] . "'";
        }

        if($param["cate_sortcode"] == "005002001") {
            $query .= " AND name IN ('아트지','스노우지','랑데뷰','아르떼','몽블랑','앙상블','뉴플러스','반누보','모조지','미색모조지','화일지','크라프트지','뉴에코크라프트') ";
        }

        $query .= " ORDER BY seq, name ";

        $rs = $conn->Execute($query);
        /*
        $temp = [];
        $temp["col"]   .= "TRIM(name) AS name, cate_sortcode";
        $temp["table"]  = "cate_paper";
        $temp["where"]["cate_sortcode"] = $param["cate_sortcode"];
        $temp["where"]["use_yn"] = "Y";
        if ($this->blankParameterCheck($param, "sort")) {
            $temp["where"]["sort"] = $param["sort"];
        }
        if ($this->blankParameterCheck($param, "link")) {
            $temp["where"]["link"] = $param["link"];
        }
        $temp["order"]  = "seq, name";

        $rs = $this->distinctData($conn, $temp);
        */
        return $rs;
    }

    /**
     * @brief 카테고리 종이정보 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     *
     * @return option html
     */
    function selectCatePaper($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]    = " mpcode";
        $temp["col"]   .= ",sort";
        $temp["col"]   .= ",name";
        $temp["col"]   .= ",dvs";
        $temp["col"]   .= ",color";
        $temp["col"]   .= ",basisweight";
        $temp["table"]  = "cate_paper";
        $temp["where"]["use_yn"] = "Y";
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $temp["where"]["cate_sortcode"] = $param["cate_sortcode"];
        }
        if ($this->blankParameterCheck($param, "mpcode")) {
            $temp["where"]["mpcode"] = $param["mpcode"];
        }
        if ($this->blankParameterCheck($param, "sort")) {
            $temp["where"]["sort"] = $param["sort"];
        }
        if ($this->blankParameterCheck($param, "name")) {
            $temp["where"]["name"] = $param["name"];
        }
        if ($this->blankParameterCheck($param, "dvs")) {
            $temp["where"]["dvs"] = $param["dvs"];
        }
        if ($this->blankParameterCheck($param, "link")) {
            $temp["like"]["link"] = $param["link"];
        }
        $temp["order"]  = "seq, dvs, name, color, CAST(basisweight AS unsigned)";

        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 카테고리 종이정보 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     * @param &$price_info_arr = 가격검색용 정보저장 배열
     *
     * @return option html
     */
    function selectCatePaperHtml($conn, $param, &$price_info_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $paper_flag = $param["paper_flag"];
        unset($param["paper_flag"]);

        $default = $param["default"];

        if (empty($default) === true) {
            $default = true;
        }

        $rs = $this->selectCatePaper($conn, $param);

        if($param['cate_sortcode'] == '003002001')
            return makeCatePaperGroupOption($rs, $default, $paper_flag, $price_info_arr, $param["express"]);
        else if($param['dvs1'] == 'sg')
            return makeCateSinglePaperOption($rs, $default, $paper_flag, $price_info_arr);
        else
            return makeCatePaperOption($rs, $default, $paper_flag, $price_info_arr);
    }

    /**
     * @brief 카테고리 인쇄도수정보 검색
     *
     * @param $conn            = connection identifier
     * @param $param           = 검색조건 파라미터
     * @param &$price_info_arr = 가격검색용 정보저장 배열
     *
     * @return 면 구분별 html 배열
     */
    function selectCatePrintTmptHtml($conn,
                                     $param,
                                     &$price_info_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $default_print = $param["default_print"];
        $default_purp  = $param["default_purp"];
        $rs = $this->selectCatePrintTmpt($conn, $param);
        if($param['dvs1'] == 'sg') {
            return makeCateSinglePrintOption($rs, $default, $paper_flag, $price_info_arr);
        } else {
            return makeCatePrintOption($rs,
                $default_print,
                $default_purp,
                $price_info_arr);
        }
    }

    /**
     * @brief 카테고리 인쇄방식 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드, 카테고리 인쇄 맵핑코드
     *
     * @return option html
     */
    function selectCatePrintPurpHtml($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_sortcode = $param["cate_sortcode"];

        $default = ProductDefaultSel::DEFAULT_SEL[$cate_sortcode]["print_purp"];
        if (empty($default) === true) {
            $default = true;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  DISTINCT A.purp_dvs";

        $query .= "\n   FROM  prdt_print AS A";
        $query .= "\n        ,cate_print AS B";

        $query .= "\n  WHERE  A.prdt_print_seqno = B.prdt_print_seqno";
        $query .= "\n    AND  B.cate_sortcode    = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode           = ";
            $query .= $param["mpcode"];
        }
        if ($this->blankParameterCheck($param, "name")) {
            $query .= "\n    AND  A.name             = ";
            $query .= $param["name"];
        }

        $query  = sprintf($query, $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        $arr = array(
            "dvs" => "purp_dvs",
            "sel" => $default
        );

        return makeOptionHtmlFront($rs, $arr);
    }

    /**
     * @brief 카테고리 사이즈정보 html 생성
     *
     * @param $conn           = connection identifier
     * @param $param          = 검색조건 파라미터
     * @param $price_info_arr = 가격검색용 정보저장 배열
     * @param $affil_yn       = 사이즈별 계열 표시 여부
     * @param $pos_yn         = 사이즈별 자리수 표시 여부
     * @param $size_typ_yn    = 사이즈 타입명 노출여부
     *
     * @return option html
     */
    function selectCateSizeHtml($conn, $param,
                                &$price_info_arr,
                                $affil_yn, $pos_yn, $size_typ_yn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        
        
        $cate_sortcode = $param["cate_sortcode"];

        $default_arr = $param["def_arr"];

        unset($param["def_arr"]);

        $default = $default_arr["size"];
        if (empty($default)) {
            $default = $param["default"];
        }
        if (empty($default)) {
            $default = true;
        }

        $pos_num_arr = 0;
        if ($pos_yn) {
            $pos_num_arr = PrdtDefaultInfo::POSITION_NUMBER[$cate_sortcode];
        }
        
        $rs = $this->selectCateSize($conn, $param);
     
        
        if($param['dvs1'] == 'sg') {
            return makeCateSingleSizeOption($rs, $default, $paper_flag, $price_info_arr);
        }
        else
            return makeCateSizeOption($rs,
                $default,
                $pos_num_arr,
                $price_info_arr,
                $affil_yn,
                $size_typ_yn);
    }

    /**
     * @brief 카테고리 수량정보 검색
     *
     * @detail $param["table_name"] = 가격 테이블명
     * @param["cate_sortcode"] = 카테고리 분류코드
     * @param["amt_unit"] = 수량단위
     *
     * @param $conn  = connection identifier
     * @param $param = 정보 배열
     * @param $price_info_arr = 가격검색용 정보저장 배열
     *
     * @return option html
     */
    function selectCateAmtHtml($conn, $param, &$price_info_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]    = "amt";
        $temp["table"]  = $param["table_name"];
        $temp["where"]["cate_sortcode"] = $param["cate_sortcode"];
        if ($this->blankParameterCheck($param, "paper_mpcode")) {
            $temp["where"]["cate_paper_mpcode"] = $param["paper_mpcode"];
        }
        if ($this->blankParameterCheck($param, "stan_mpcode")) {
            $temp["where"]["cate_stan_mpcode"] = $param["stan_mpcode"];
        }
        $temp["order"]  = "(amt + 0)";
        $rs = $this->distinctData($conn, $temp);

        return makeCateAmtOption($rs,
                                 $param["amt_unit"],
                                 $param["def_amt"],
                                 $price_info_arr,
                                 $param["divide"]);
    }

    /**
     * @brief 카테고리 포장방식 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return option html
     */
    function selectCatePackWayHtml($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_sortcode = $this->parameterEscape($conn, $cate_sortcode);

        $query  = "\n SELECT  A.pack_name";

        $query .= "\n   FROM  pack_way      AS A";
        $query .= "\n        ,cate_pack_way AS B";

        $query .= "\n  WHERE  A.pack_way_seqno = B.pack_way_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";

        $query  = sprintf($query, $cate_sortcode);

        $rs = $conn->Execute($query);

        return makeCatePackWayOption($rs);
    }

    /**
     * @brief 카테고리 옵션 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드, 상품구분값
     *
     * @return ul html
     */
    function selectCateOptHtml($conn, $param, $def_arr = []) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_sortcode = $param["cate_sortcode"];
        $cate_sortcode = $this->parameterEscape($conn, $cate_sortcode);

        $query  = "\n SELECT  A.opt_name AS name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,B.mpcode";
        $query .= "\n        ,B.basic_yn";
        $query .= "\n        ,B.seq";
        $query .= "\n        ,B.cate_sortcode";   // 합판전단때문에 추가

        $query .= "\n   FROM  prdt_opt AS A";
        $query .= "\n        ,cate_opt AS B";

        $query .= "\n  WHERE  A.prdt_opt_seqno = B.prdt_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n  GROUP  BY opt_name";
        $query .= "\n  ORDER  BY seq ASC";

        $query  = sprintf($query, $cate_sortcode);
        $rs = $conn->Execute($query);

        $info_arr = array();
        $info_arr["dvs"] = $param["dvs"];
        $html = makeCateOptUl($rs, $info_arr, $def_arr, $cate_sortcode, $param['link']);
        unset($info_arr["dvs"]);

        return array("info_arr" => $info_arr,
                     "html"     => $html);
    }

    /**
     * @brief 카테고리 추가 옵션 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return option html
     */
    function selectCateAddOptInfoHtml($conn, $param, $def_arr = []) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mobile_yn = $param["mobile_yn"];

        $cate_sortcode = $this->parameterEscape($conn, $param["cate_sortcode"]);

        $query  = "\n SELECT  A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  prdt_opt AS A";
        $query .= "\n        ,cate_opt AS B";

        $query .= "\n  WHERE  A.prdt_opt_seqno = B.prdt_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = 'N'";
        $query .= "\n    AND  A.opt_name IN (%s)";

        $query  = sprintf($query, $cate_sortcode, $param["opt_name"]);
        $rs = $conn->Execute($query);
        return makeCateAddOpt($rs,
                              $param["opt_idx"],
                              $param["dvs"],
                              $mobile_yn,
                              false,
                              $def_arr);
    }

    /**
     * @brief 카테고리 후공정 검색
     *
     * @param $conn          = connection identifier
     * @param $param         = 카테고리 분류코드, 제품 구분값
     * @param $except_arr    = 검색제외배열
     *
     * @return ul html
     */
    function selectCateAfterHtml($conn, $param, $def_arr = [], $except_arr = []) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $dvs = $param["dvs"];

        $query  = "\n SELECT  A.after_name AS name";
        $query .= "\n        ,B.basic_yn";

        $query .= "\n   FROM  prdt_after AS A";
        $query .= "\n        ,cate_after AS B";

        $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n  ORDER BY B.seq, A.after_name ";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"]));
        $rs = $conn->Execute($query, [$param["cate_sortcode"]]);
        $info_arr = [];
        $html = makeCateAftUl($rs, $dvs, $info_arr, $def_arr, $except_arr);

        return array("info_arr" => $info_arr,
                     "html"     => $html);
    }

    /**
     * @brief 카테고리 추가 후공정 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     * @param $except_arr    = html 생성 제외 후공정
     *
     * @return option html
     */
    function selectCateAddAfterInfoHtml($conn,
                                        $param,
                                        $def_arr = [],
                                        $except_arr = [],
                                        $foil_count = "") {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mobile_yn = $param["mobile_yn"];
        $dvs = $param["dvs"];

        $size = $this->parameterEscape($conn, $param["size"]);

        $query  = "\n    SELECT  A.after_name";
        $query .= "\n           ,A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,B.mpcode";

        $query .= "\n      FROM  prdt_after AS A";
        $query .= "\n           ,cate_after AS B";

        $query .= "\n     WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  B.basic_yn = 'N'";
        $query .= "\n       AND  A.after_name = %s";
        if ($this->blankParameterCheck($param, "size")) {
            $query .= "\n       AND  B.size = " . $size;
        }
        $query .= "\n  ORDER BY  A.tab_index , A.depth1, A.depth2, B.seq";
        $query .= "\n           ,A.after_name";
        $query .= "\n           ,A.prdt_after_seqno";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                , $conn->param($param["after_name"]));
//echo $query;
        $temp = [];
        $temp[] = $param["cate_sortcode"];
        $temp[] = $param["after_name"];
        $rs = $conn->Execute($query, $temp);
        if ($rs->EOF) {
            $query  = "\n    SELECT  A.after_name";
            $query .= "\n           ,A.depth1";
            $query .= "\n           ,A.depth2";
            $query .= "\n           ,A.depth3";
            $query .= "\n           ,B.mpcode";

            $query .= "\n      FROM  prdt_after AS A";
            $query .= "\n           ,cate_after AS B";

            $query .= "\n     WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
            $query .= "\n       AND  B.cate_sortcode = %s";
            $query .= "\n       AND  B.basic_yn = 'N'";
            $query .= "\n       AND  A.after_name = %s";

            $query .= "\n  ORDER BY  A.tab_index, A.depth1, A.depth2, B.seq";
            $query .= "\n           ,A.after_name";
            $query .= "\n           ,A.prdt_after_seqno";

            $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                    , $conn->param($param["after_name"]));

            unset($temp);
            $temp[] = $param["cate_sortcode"];
            $temp[] = $param["after_name"];
            $rs = $conn->Execute($query, $temp);
        }

        return makeCateAddAfter($rs, $dvs, $mobile_yn, $def_arr, $except_arr, $foil_count);
    }

    /**
     * @brief 종이느낌 검색
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 종이 맵핑코드
     *
     * @return option html
     */
    function selectPaperDscr($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "name, dvs";
        $temp["table"] = "cate_paper";
        $temp["where"]["mpcode"] = $mpcode;

        $rs = $this->selectData($conn, $temp);

        $name = $rs->fields["name"];
        $dvs  = $rs->fields["dvs"];
        unset($rs);
        unset($temp);

        $temp["col"]   = "paper_sense";
        $temp["table"] = "paper_dscr";
        $temp["where"]["name"] = $name;
        $temp["where"]["dvs"]  = $dvs;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["paper_sense"];
    }

    /**
     * @brief 회원 등급별 할인 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["grade"] = 회원 등급
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return option html
     */
    function selectGradeSaleRate($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]   = "rate";
        $temp["table"] = "grade_sale_price";
        $temp["where"]["cate_sortcode"] = $param["cate_sortcode"];
        $temp["where"]["grade"]         = $param["grade"];

        $rs = $this->selectData($conn, $temp);

        $rate = $rs->fields["rate"];
        if (empty($rate) === true) {
            $rate = 0;
        }

        return doubleval($rate * -1);
    }

    /**
     * @brief 상품 확정형 가격 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 중분류코드
     * $param["paper_mpcode"] = 카테고리 종이 맵핑코드
     * $param["cate_beforeside_print_mpcode"] = 카테고리 전면 인쇄 맵핑코드
     * $param["cate_beforeside_add_print_mpcode"] = 카테고리 전면 추가 인쇄 맵핑코드
     * $param["cate_aftside_print_mpcode"] = 카테고리 후면 인쇄 맵핑코드
     * $param["cate_aftside_add_print_mpcode"] = 카테고리 후면 추가 인쇄 맵핑코드
     * $param["stan_mpcode"] = 카테고리 규격 맵핑코드
     * $param["amt"] = 수량
     * $param["table_name"] = 가격 테이블명
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 신규가격
     */
    function selectPrdtPlyPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $table_name = $param["table_name"];

        if (empty($table_name)) {
            $table_name = "ply_price";
        }

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query_common  = "\n SELECT  new_price";
        $query_common .= "\n        ,page";
        $query_common .= "\n        ,page_dvs";

        $query_common .= "\n   FROM  %s";

        $query_common .= "\n  WHERE  cate_sortcode                    = %s";
        $query_common .= "\n    AND  cate_paper_mpcode                = %s";
        $query_common .= "\n    AND  cate_beforeside_print_mpcode     = %s";
        $query_common .= "\n    AND  cate_beforeside_add_print_mpcode = %s";
        $query_common .= "\n    AND  cate_aftside_print_mpcode        = %s";
        $query_common .= "\n    AND  cate_aftside_add_print_mpcode    = %s";
        $query_common .= "\n    AND  cate_stan_mpcode                 = %s";
        $query_common .= "\n    AND  new_price                       != 0";


        $query_common  = sprintf($query_common, $table_name
                                              , $param["cate_sortcode"]
                                              , $param["paper_mpcode"]
                                              , $param["bef_print_mpcode"]
                                              , $param["bef_add_print_mpcode"]
                                              , $param["aft_print_mpcode"]
                                              , $param["aft_add_print_mpcode"]
                                              , $param["stan_mpcode"]);

        if ($this->blankParameterCheck($param, "page")) {
            $query_common .= "\n    AND  page = %s" . $param["page"];
        }
        if ($this->blankParameterCheck($param, "page_dvs")) {
            $query_common .= "\n    AND  page_dvs = " . $param["page_dvs"];
        }
        if ($this->blankParameterCheck($param, "page_detail")) {
            $query_common .= "\n    AND  page_detail = " . $param["page_detail"];
        }

        $query  = $query_common;

        $query .= "\n    AND  " . $param["amt"] . " <= (amt + 0)";
        $query .= "\n  ORDER BY (amt + 0)";
        $query .= "\n  LIMIT  1";

        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $query = $query_common;

            $query .= "\n ORDER BY  (amt + 0) DESC";
            $query .= "\n    LIMIT  1";

            $rs = $conn->Execute($query);

        }

        return $rs->fields;
    }

    function selectPrdtPlyPrice2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $table_name = $param["table_name"];

        if (empty($table_name)) {
            $table_name = "ply_price";
        }

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query_common  = "\n SELECT  new_price";
        $query_common .= "\n        ,page";
        $query_common .= "\n        ,page_dvs";

        $query_common .= "\n   FROM  %s";

        $query_common .= "\n  WHERE  cate_sortcode                    = %s";
        $query_common .= "\n    AND  cate_paper_mpcode                = %s";
        $query_common .= "\n    AND  cate_beforeside_print_mpcode     = %s";
        $query_common .= "\n    AND  cate_beforeside_add_print_mpcode = %s";
        $query_common .= "\n    AND  cate_aftside_print_mpcode        = %s";
        $query_common .= "\n    AND  cate_aftside_add_print_mpcode    = %s";
        $query_common .= "\n    AND  cate_stan_mpcode                 = %s";
        $query_common .= "\n    AND  new_price                       != 0";


        $query_common  = sprintf($query_common, $table_name
                                              , $param["cate_sortcode"]
                                              , $param["paper_mpcode"]
                                              , $param["bef_print_mpcode"]
                                              , $param["bef_add_print_mpcode"]
                                              , $param["aft_print_mpcode"]
                                              , $param["aft_add_print_mpcode"]
                                              , $param["stan_mpcode"]);

        if ($this->blankParameterCheck($param, "page")) {
            $query_common .= "\n    AND  page = %s" . $param["page"];
        }
        if ($this->blankParameterCheck($param, "page_dvs")) {
            $query_common .= "\n    AND  page_dvs = " . $param["page_dvs"];
        }
        if ($this->blankParameterCheck($param, "page_detail")) {
            $query_common .= "\n    AND  page_detail = " . $param["page_detail"];
        }

        $query  = $query_common;

        $query .= "\n    AND  " . $param["amt"] . " <= (amt + 0)";
        $query .= "\n  ORDER BY (amt + 0)";
        $query .= "\n  LIMIT  1";

     /*   $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $query = $query_common;

            $query .= "\n ORDER BY  (amt + 0) DESC";
            $query .= "\n    LIMIT  1";

            $rs = $conn->Execute($query);

        }
        */

        return $query;
    }

    function selectPriceRate($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  B.rate, A.basic_rate";
        $query .= "\n   FROM  cpn_admin AS A ";
        $query .= "\n   LEFT JOIN channel_price_rate AS B ON A.sell_site = B.sell_site AND B.cate_sortcode = %s ";
        $query .= "\n  WHERE  A.sell_site = %s";



        $query  = sprintf($query
            , $param["sortcode"]
            , $param["sell_site"]);
        $rs = $conn->Execute($query);
        return $rs->fields;

    }

    function selectA3PricePer($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query_common  = "\n SELECT  sell_price";
        $query_common .= "\n   FROM  cate_paper AS A ";
        $query_common .= "\n   INNER JOIN prdt_paper AS B ON A.name = B.name AND A.dvs = B.dvs AND A.color = B.color AND A.basisweight = CONCAT(B.basisweight,B.basisweight_unit) AND B.affil = 'A3' AND A.sort = B.sort ";
        $query_common .= "\n   INNER JOIN prdt_paper_price AS C ON B.mpcode = C.prdt_paper_mpcode ";

        $query_common .= "\n  WHERE  A.cate_sortcode = %s";
        $query_common .= "\n    AND  A.mpcode = %s";



        $query_common  = sprintf($query_common
            , $param["cate_sortcode"]
            , $param["paper_mpcode"]);

        $query  = $query_common;

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    /**
     * @brief 카테고리 후공정 맵핑코드/기준수량 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["after_name"] = 후공정명
     * $param["depth1"] = 후공정 depth1
     * $param["depth2"] = 후공정 depth2
     * $param["depth3"] = 후공정 depth3
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateAfterInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, ["mpcode" => true]);

        $query  = "\n SELECT  B.mpcode";
        $query .= "\n        ,A.after_name";
        $query .= "\n        ,B.basic_yn";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.crtr_unit";
        $query .= "\n   FROM  prdt_after AS A";
        $query .= "\n        ,cate_after AS B";
        $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        if ($this->blankParameterCheck($param, "after_name")) {
            $query .= "\n    AND  A.after_name = " . $param["after_name"];
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  B.cate_sortcode = " . $param["cate_sortcode"];
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = " . $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = " . $param["depth2"];
        }
        if ($this->blankParameterCheck($param, "depth3")) {
            $query .= "\n    AND  A.depth3 = " . $param["depth3"];
        }
        if ($this->blankParameterCheck($param, "size")) {
            $query .= "\n    AND  B.size = " . $param["size"];
        }
        if ($this->blankParameterCheck($param, "basic_yn")) {
            $query .= "\n    AND  B.basic_yn = " . $param["basic_yn"];
        }
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }
        $query .= "\n  ORDER BY A.tab_index ";
        $rs = $conn->Execute($query);

        /*
        if ($rs->EOF) {
            $query  = "\n SELECT  B.mpcode";
            $query .= "\n        ,A.after_name";
            $query .= "\n        ,A.depth1";
            $query .= "\n        ,A.depth2";
            $query .= "\n        ,A.depth3";
            $query .= "\n        ,B.crtr_unit";
            $query .= "\n   FROM  prdt_after AS A";
            $query .= "\n        ,cate_after AS B";
            $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
            if ($this->blankParameterCheck($param, "after_name")) {
                $query .= "\n    AND  A.after_name = " . $param["after_name"];
            }
            if ($this->blankParameterCheck($param, "cate_sortcode")) {
                $query .= "\n    AND  B.cate_sortcode = " . $param["cate_sortcode"];
            }
            if ($this->blankParameterCheck($param, "depth1")) {
                $query .= "\n    AND  A.depth1 = " . $param["depth1"];
            }
            if ($this->blankParameterCheck($param, "depth2")) {
                $query .= "\n    AND  A.depth2 = " . $param["depth2"];
            }
            if ($this->blankParameterCheck($param, "depth3")) {
                $query .= "\n    AND  A.depth3 = " . $param["depth3"];
            }
            if ($this->blankParameterCheck($param, "mpcode")) {
                $query .= "\n    AND  B.mpcode IN (";
                $query .= $param["mpcode"];
                $query .= ')';
            }

            $rs = $conn->Execute($query);
        }
*/
        return $rs;
    }


    function selectCateAoAfterInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param, ["mpcode" => true]);

        $query  = "\n SELECT  B.mpcode";
        $query .= "\n        ,A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.crtr_unit";
        $query .= "\n        ,B.basic_yn";
        $query .= "\n   FROM  ao_after AS A";
        $query .= "\n        ,cate_ao_after AS B";
        $query .= "\n  WHERE  A.ao_after_seqno = B.ao_after_seqno ";
        if ($this->blankParameterCheck($param, "after_name")) {
            $query .= "\n    AND  A.after_name = " . $param["after_name"];
        }
        if ($this->blankParameterCheck($param, "cate_sortcode")) {
            $query .= "\n    AND  B.cate_sortcode = " . $param["cate_sortcode"];
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = " . $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = " . $param["depth2"];
        }
        if ($this->blankParameterCheck($param, "depth3")) {
            $query .= "\n    AND  A.depth3 = " . $param["depth3"];
        }
        if ($this->blankParameterCheck($param, "size")) {
            $query .= "\n    AND  B.size = " . $param["size"];
        }
        if ($this->blankParameterCheck($param, "basic_yn")) {
            $query .= "\n    AND  B.basic_yn = " . $param["basic_yn"];
        }
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }

        $rs = $conn->Execute($query);

        /*
        if ($rs->EOF) {
            $query  = "\n SELECT  B.mpcode";
            $query .= "\n        ,A.after_name";
            $query .= "\n        ,A.depth1";
            $query .= "\n        ,A.depth2";
            $query .= "\n        ,A.depth3";
            $query .= "\n        ,B.crtr_unit";
            $query .= "\n   FROM  ao_after AS A";
            $query .= "\n        ,cate_ao_after AS B";
            $query .= "\n  WHERE  A.ao_after_seqno = B.ao_after_seqno ";
            if ($this->blankParameterCheck($param, "after_name")) {
                $query .= "\n    AND  A.after_name = " . $param["after_name"];
            }
            if ($this->blankParameterCheck($param, "cate_sortcode")) {
                $query .= "\n    AND  B.cate_sortcode = " . $param["cate_sortcode"];
            }
            if ($this->blankParameterCheck($param, "depth1")) {
                $query .= "\n    AND  A.depth1 = " . $param["depth1"];
            }
            if ($this->blankParameterCheck($param, "depth2")) {
                $query .= "\n    AND  A.depth2 = " . $param["depth2"];
            }
            if ($this->blankParameterCheck($param, "depth3")) {
                $query .= "\n    AND  A.depth3 = " . $param["depth3"];
            }
            if ($this->blankParameterCheck($param, "mpcode")) {
                $query .= "\n    AND  B.mpcode IN (";
                $query .= $param["mpcode"];
                $query .= ')';
            }

            $rs = $conn->Execute($query);
        }
        */

        return $rs;
    }

    /**
     * @brief 카테고리 후공정 하위 depth 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["after_name"] = 후공정명
     * $param["depth1"] = 후공정 depth1
     * $param["depth2"] = 후공정 depth2
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $flag  = 맵핑코드 검색여부
     *
     * @return option html
     */
    function selectCateAfterDepthHtml($conn, $param, $flag = true) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $after_name = $param["after_name"];

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "";

        if ($this->blankParameterCheck($param, "after_name")) {
            $query  = "\n SELECT  A.depth1 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query  = "\n SELECT  A.depth2 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query  = "\n SELECT  A.depth3 AS lower_depth";
        }
        if ($flag === true) {
            $query .= "\n        ,B.mpcode";
        }
        $query .= "\n   FROM  prdt_after AS A";
        $query .= "\n        ,cate_after AS B";
        $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n    AND  A.after_name = %s";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = " . $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = " . $param["depth2"];
        }
        if ($this->blankParameterCheck($param, "size")) {
            $query .= "\n    AND  B.size = " . $param["size"];
        }
        $query .= "\n    ORDER BY A.tab_index ";
        $query  = sprintf($query, $param["after_name"]
                                , $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            if ($this->blankParameterCheck($param, "after_name")) {
                $query  = "\n SELECT  A.depth1 AS lower_depth";
            }
            if ($this->blankParameterCheck($param, "depth1")) {
                $query  = "\n SELECT  A.depth2 AS lower_depth";
            }
            if ($this->blankParameterCheck($param, "depth2")) {
                $query  = "\n SELECT  A.depth3 AS lower_depth";
            }
            if ($flag === true) {
                $query .= "\n        ,B.mpcode";
            }
            $query .= "\n   FROM  prdt_after AS A";
            $query .= "\n        ,cate_after AS B";
            $query .= "\n  WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
            $query .= "\n    AND  A.after_name = %s";
            $query .= "\n    AND  B.cate_sortcode = %s";
            if ($this->blankParameterCheck($param, "depth1")) {
                $query .= "\n    AND  A.depth1 = " . $param["depth1"];
            }
            if ($this->blankParameterCheck($param, "depth2")) {
                $query .= "\n    AND  A.depth2 = " . $param["depth2"];
            }
            $query .= "\n    ORDER BY A.tab_index ";
            $query  = sprintf($query, $param["after_name"]
                                    , $param["cate_sortcode"]);

            $rs = $conn->Execute($query);
        }

        $arr = array(
            "val"        => "mpcode",
            "dvs"        => "lower_depth",
            "except_arr" => array(
                "after_name" => $after_name
            )
        );

        return makeOptionHtmlFront($rs, $arr);
    }

    /**
     * @brief 카테고리 후공정 가격 검색
     *
     * @detail $param["mpcode"] = 카테고리 후공정 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateAfterPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = ["mpcode" => true];

        $amt = $param["amt"];
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.amt";
        $query .= "\n        ,A.sell_price";
        $query .= "\n        ,A.cate_after_mpcode AS mpcode";
        $query .= "\n   FROM  cate_after_price AS A";
        $query .= "\n  WHERE  A.cate_after_mpcode IN (%s)";
        if ($this->blankParameterCheck($param, "amt")) {
            $query .= "\n    AND  " . $amt . " <= (A.amt + 0) ";
            $query .= "\n    ORDER BY (A.amt + 0) ASC ";
            $query .= "\n    LIMIT 1 ";
        }

        $query  = sprintf($query, $param["mpcode"]);

        return $conn->Execute($query);

    }

    function selectAfterDepth($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  depth1, depth2 FROM cate_after AS A ";
        $query .= "\n INNER JOIN prdt_after AS B ON A.prdt_after_seqno = B.prdt_after_seqno ";
        $query .= "\n WHERE A.mpcode = %s ";

        $query  = sprintf($query, $param["mpcode"]);

        return $conn->Execute($query);

    }

    function selectTheSizeMpcode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT D.mpcode FROM cate_after AS A ";
        $query .= "\n INNER JOIN prdt_after AS B ON A.prdt_after_seqno = B.prdt_after_seqno ";
        $query .= "\n INNER JOIN prdt_after AS C ON B.after_name = C.after_name AND B.depth1 = C.depth1 AND B.depth2 = C.depth2 ";
        $query .= "\n INNER JOIN cate_after AS D ON D.prdt_after_seqno = C.prdt_after_seqno AND D.size = %s ";
        $query .= "\n WHERE A.mpcode = %s ";

        $query  = sprintf($query, $param["size"], $param["mpcode"]);
        return $conn->Execute($query);

    }

    /**
     * @brief 카테고리 옵션 맵핑코드/기준수량 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["opt_name"] = 후공정명
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateOptInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  B.mpcode";
        $query .= "\n        ,A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n   FROM  prdt_opt AS A";
        $query .= "\n        ,cate_opt AS B";
        $query .= "\n  WHERE  A.prdt_opt_seqno = B.prdt_opt_seqno";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n    AND  B.mpcode         = ";
            $query .= $param["mpcode"];
        } else if ($this->blankParameterCheck($param, "opt_name")) {
            $query .= "\n    AND  A.opt_name         = ";
            $query .= $param["name"];
        } else {
            $query .= "\n    AND  B.basic_yn       = %s";
            $query .= "\n    AND  B.cate_sortcode  = %s";

            $query  = sprintf($query, $param["basic_yn"]
                                    , $param["cate_sortcode"]);
        }


        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 옵션 가격 검색
     *
     * @detail $param["mpcode"] = 카테고리 후공정 맵핑코드
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 후공정 맵핑코드
     *
     * @return 검색결과
     */
    function selectCateOptPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("mpcode" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.amt";
        $query .= "\n        ,A.sell_price";
        $query .= "\n        ,A.cate_opt_mpcode AS mpcode";
        $query .= "\n   FROM  cate_opt_price AS A";
        $query .= "\n  WHERE  A.cate_opt_mpcode IN (%s)";

        $query  = sprintf($query, $param["mpcode"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 카테고리 옵션 가격 검색 단일
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["basic_yn"] = 기본여부
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateOptSinglePrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  B.sell_price";

        $query .= "\n   FROM  cate_opt       AS A";
        $query .= "\n        ,cate_opt_price AS B";

        $query .= "\n  WHERE  A.mpcode = B.cate_opt_mpcode";
        $query .= "\n    AND  A.cate_sortcode   = %s";
        $query .= "\n    AND  A.basic_yn        = %s";
        $query .= "\n  LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]);

        $rs = $conn->Execute($query);

        return intval($rs->fields["sell_price"]);
    }

    /**
     * @brief 카테고리 후공정 가격 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["basic_yn"] = 기본여부
     * $param["except_after"] = 검색제외 후공정명
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateAfterSinglePrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  C.sell_price";

        $query .= "\n      FROM  prdt_after       AS A";
        $query .= "\n           ,cate_after       AS B";
        $query .= "\n           ,cate_after_price AS C";

        $query .= "\n     WHERE  A.prdt_after_seqno = B.prdt_after_seqno";
        $query .= "\n       AND  B.mpcode = C.cate_after_mpcode";
        if ($this->blankParameterCheck($param, "except_after")) {
            $query .= "\n       AND  A.after_name != " . $param["except_after"];
        }
        $query .= "\n       AND  B.cate_sortcode   = %s";
        $query .= "\n       AND  B.basic_yn        = %s";
        $query .= "\n       AND  %s <= (C.amt + 0)";
        $query .= "\n  ORDER BY  (C.amt + 0) ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["basic_yn"]
                                , $param["amt"]);

        $rs = $conn->Execute($query);

        // 해당하는 수량이 없을경우 제일 마지막 수량 판매가격 반환
        if ($rs->EOF) {
            $query  = "\n    SELECT  B.sell_price";

            $query .= "\n      FROM  cate_after       AS A";
            $query .= "\n           ,cate_after_price AS B";

            $query .= "\n     WHERE  A.mpcode = B.cate_after_mpcode";
            $query .= "\n       AND  A.cate_sortcode   = %s";
            $query .= "\n       AND  A.basic_yn        = %s";
            $query .= "\n  ORDER BY  (B.amt + 0) DESC";
            $query .= "\n     LIMIT  1";

            $query  = sprintf($query, $param["cate_sortcode"]
                                    , $param["basic_yn"]);

            $rs = $conn->Execute($query);
        }

        $ret = intval($rs->fields["sell_price"]);

        if ($rs->EOF) {
            $ret = 0;
        }

        return $ret;
    }

    /**
     * @brief 종이 가격 검색
     *
     * @detail @param["mpcode"] = 상품 종이 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색정보 파라미터
     *
     * @return 종이 판매가격
     */
    function selectPaperPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "sell_price";
        $temp["table"] = "prdt_paper_price";
        $temp["where"]["prdt_paper_mpcode"] = $param["mpcode"];

        $rs = $this->selectData($conn, $temp);

        return intval($rs->fields["sell_price"]);
    }

    /**
     * @brief 인쇄 가격 검색
     *
     * @detail @param["mpcode"] = 상품 종이 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색정보 파라미터
     *
     * @return 검색결과
     */
    function selectPrintPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("amt" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n    SELECT  sell_price";
        $query .= "\n      FROM  prdt_print_price";
        $query .= "\n     WHERE  prdt_print_info_mpcode = %s";
        $query .= "\n       AND  %s <= (amt + 0)";
        $query .= "\n  ORDER BY  (amt + 0) ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["mpcode"]
                                , $param["amt"]);

        $rs = $conn->Execute($query);

        // 해당하는 수량이 없을경우 제일 마지막 수량 판매가격 반환
        if ($rs->EOF) {
            $query  = "\n    SELECT  sell_price";
            $query .= "\n      FROM  prdt_print_price";
            $query .= "\n     WHERE  prdt_print_info_mpcode = %s";
            $query .= "\n  ORDER BY  (amt + 0) DESC";
            $query .= "\n     LIMIT  1";

            $query  = sprintf($query, $param["mpcode"]);

            $rs = $conn->Execute($query);
        }

        return intval($rs->fields["sell_price"]);
    }

    /**
     * @brief 인쇄 최소수량 검색
     *
     * @detail @param["mpcode"] = 상품 종이 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색정보 파라미터
     *
     * @return 검색결과
     */
    function selectPrintMinAmt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n    SELECT  MIN(amt + 0) AS min_amt";
        $query .= "\n      FROM  prdt_print_price";
        $query .= "\n     WHERE  prdt_print_info_mpcode = %s";

        $query  = sprintf($query, $param["mpcode"]);
                                
        $rs = $conn->Execute($query);

        return doubleval($rs->fields["min_amt"]);
    }

    /**
     * @brief 출력 가격 검색
     *
     * @detail @param["mpcode"] = 상품 종이 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색정보 파라미터
     *
     * @return 출력 판매가격
     */
    function selectOutputPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  sell_price";
        $query .= "\n      FROM  prdt_stan_price";
        $query .= "\n     WHERE  prdt_output_info_mpcode = %s";
        //$query .= "\n       AND  board_amt = '1'";

        $query  = sprintf($query, $param["mpcode"]);

        $rs = $conn->Execute($query);

        return intval($rs->fields["sell_price"]);
    }

    /**
     * @brief 상품_출력_정보 맵핑코드 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * @param["mpcode"] = 상품 규격 맵핑코드
     *
     * @param $conn  = connection identifier
     * @param $param = 검색정보 파라미터
     *
     * @return 상품 인쇄 정보
     */
    function selectPrdtOutputMpcode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  B.mpcode";

        $query .= "\n   FROM  prdt_stan          AS A";
        $query .= "\n        ,prdt_output_info   AS B";
        $query .= "\n        ,cate_stan          AS C";

        $query .= "\n  WHERE  A.prdt_stan_seqno  = C.prdt_stan_seqno";
        $query .= "\n    AND  A.output_name      = B.output_name";
        $query .= "\n    AND  A.output_board_dvs = B.output_board_dvs";
        $query .= "\n    AND  C.cate_sortcode    = %s";
        $query .= "\n    AND  C.mpcode           = %s";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["mpcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields["mpcode"];
    }

    /**
     * @brief 하위 카테고리 분류코드 중 가장 작은 값 검색
     *
     * @param $conn = connection identifier
     * @param $sortcode = 상위 카테고리 분류코드
     *
     * @return 하위 카테고리 분류코드
     */
    function selectCateSortcode($conn, $high_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"] = "MIN(A.sortcode) AS sortcode";
        $temp["table"] = "cate AS A";
        $temp["where"]["A.high_sortcode"] = $high_sortcode;
        $temp["where"]["A.use_yn"] = 'Y';

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["sortcode"];
    }

    /**
     * @brief 카테고리명 검색 후 option html 생성
     *
     * @detail 혼합형 주문페이지에서만 사용
     *
     * @param $conn         = connection identifier
     * @param $sortcode     = 검색조건 분류코드
     *
     * @return 검색결과
     */
    function selectMixCateHtml($conn, $sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_name = $this->selectCateName($conn, $sortcode);

        return option($sortcode, $cate_name);
    }

    /**
     * @brief 근접한 규격사이즈 검색
     *
     * @param $conn     = connection identifier
     * @param $sortcode = 검색조건 분류코드
     *
     * @return 검색결과
     */
    function selectSimilarStanInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = ["mpcode" => true];
        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n    SELECT  A.name";
        $query .= "\n           ,A.affil";
        $query .= "\n           ,A.cut_wid_size";
        $query .= "\n           ,A.cut_vert_size";
        $query .= "\n           ,B.mpcode";

        $query .= "\n      FROM  prdt_stan AS A";
        $query .= "\n           ,cate_stan AS B";

        $query .= "\n     WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "mpcode")) {
            $query .= "\n       AND  B.mpcode IN (";
            $query .= $param["mpcode"];
            $query .= ')';
        }
        $query .= "\n       AND  ((A.cut_wid_size  = %s";
        $query .= "\n              AND  A.cut_vert_size = %s)";
        $query .= "\n               OR  (A.cut_vert_size  = %s";
        $query .= "\n              AND  A.cut_wid_size = %s))";

        $query .= "\n  ORDER BY  A.cut_wid_size ASC, A.cut_vert_size ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["wid"]
                                , $param["vert"]
                                , $param["wid"]
                                , $param["vert"]);

        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $query  = "\n    SELECT  A.name";
            $query .= "\n           ,A.affil";
            $query .= "\n           ,A.cut_wid_size";
            $query .= "\n           ,A.cut_vert_size";
            $query .= "\n           ,B.mpcode";

            $query .= "\n      FROM  prdt_stan AS A";
            $query .= "\n           ,cate_stan AS B";

            $query .= "\n     WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
            $query .= "\n       AND  B.cate_sortcode = %s";
            $query .= "\n       AND  ((A.cut_wid_size  >= %s";
            $query .= "\n              AND  A.cut_vert_size >= %s)";
            $query .= "\n               OR  (A.cut_vert_size  >= %s";
            $query .= "\n              AND  A.cut_wid_size >= %s))";

            $query .= "\n  ORDER BY  A.cut_wid_size ASC, A.cut_vert_size ASC";
            $query .= "\n     LIMIT  1";

            $query  = sprintf($query, $param["cate_sortcode"]
                                    , $param["wid"]
                                    , $param["vert"]
                                    , $param["wid"]
                                    , $param["vert"]);

            $rs = $conn->Execute($query);
        }

        return $rs->fields;
    }

    /**
     * @brief 해당 카테고리의 최대 규격사이즈 검색
     *
     * @param $conn     = connection identifier
     * @param $sortcode = 검색조건 분류코드
     *
     * @return 검색결과
     */
    function selectMaxStanInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr = ["mpcode" => true];

        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\n    SELECT  A.name";
        $query .= "\n           ,A.affil";
        $query .= "\n           ,B.mpcode";
        $query .= "\n           ,A.cut_wid_size";
        $query .= "\n           ,A.cut_vert_size";

        $query .= "\n      FROM  prdt_stan AS A";
        $query .= "\n           ,cate_stan AS B";

        $query .= "\n     WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  B.mpcode IN (%s)";
        $query .= "\n  ORDER BY  A.cut_wid_size + A.cut_vert_size DESC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["mpcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 해당 카테고리 사이즈의 재단/작업 사이즈 검색
     *
     * @param $conn     = connection identifier
     * @param $sortcode = 검색조건 분류코드
     *
     * @return 검색결과
     */
    function selectCutWorkStanInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  /* 재단, 작업 사이즈 검색 */";
        $query .= "\n            A.cut_wid_size";
        $query .= "\n           ,A.cut_vert_size";
        $query .= "\n           ,A.work_wid_size";
        $query .= "\n           ,A.work_vert_size";

        $query .= "\n      FROM  prdt_stan AS A";
        $query .= "\n           ,cate_stan AS B";

        $query .= "\n     WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  A.name          = %s";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["stan_name"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 후공정, 박 가격 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterFoilPressPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  B.sell_price as price";

        $query .= "\n      FROM cate_after AS A ";
        $query .= "\n      INNER JOIN cate_after_price AS B ON A.mpcode = B.cate_after_mpcode ";
        $query .= "\n      INNER JOIN prdt_after AS C ON A.prdt_after_seqno = C.prdt_after_seqno ";
        $query .= "\n     WHERE  A.cate_sortcode = %s";
        $query .= "\n       AND  C.after_name    = %s";
        $query .= "\n       AND  C.depth1    = %s";
        $query .= "\n       AND  C.depth2           = %s";
        if ($this->blankParameterCheck($param, "size_name")) {
            $query .= "\n    AND  A.size = " . $param["size_name"];
        }
        //$query .= "\n       AND  %s <= (B.amt + 0)";
        $query .= "\n  ORDER BY  (B.amt + 0) ASC, (B.sell_price + 0) ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["after_name"]
                                , $param["depth1"]
                                , $param["depth2"]
                                );
        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $query  = "\n    SELECT  A.price";

            $query .= "\n      FROM  after_foil_press_price AS A";

            $query .= "\n     WHERE  A.cate_sortcode = %s";
            $query .= "\n       AND  A.after_name    = %s";
            $query .= "\n       AND  A.dvs           = %s";
            $query .= "\n  ORDER BY  (amt + 0) DESC";
            $query .= "\n     LIMIT  1";

            $query  = sprintf($query, $param["cate_sortcode"]
                                    , $param["after_name"]
                                    , $param["dvs"]);

            $rs = $conn->Execute($query);
        }
        return intval($rs->fields["price"]);
    }

    function selectAfterEmbossingPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  B.sell_price as price, B.amt";

        $query .= "\n      FROM cate_after AS A ";
        $query .= "\n      INNER JOIN cate_after_price AS B ON A.mpcode = B.cate_after_mpcode ";
        $query .= "\n      INNER JOIN prdt_after AS C ON A.prdt_after_seqno = C.prdt_after_seqno ";
        $query .= "\n     WHERE  A.cate_sortcode = %s";
        $query .= "\n       AND  C.after_name    = %s";
        $query .= "\n       AND  C.depth1    = %s";
        $query .= "\n       AND  C.depth2           = %s";
        if ($this->blankParameterCheck($param, "size_name")) {
            $query .= "\n    AND  A.size = " . $param["size_name"];
        }
        $query .= "\n       AND  %s <= (B.amt + 0)";
        $query .= "\n  ORDER BY  (B.amt + 0) ASC, (B.sell_price + 0) ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["cate_sortcode"]
            , $param["after_name"]
            , $param["depth1"]
            , $param["depth2"]
            , $param["amt"]);
        $rs = $conn->Execute($query);

        if ($rs->EOF) {
            $query  = "\n    SELECT  A.price";

            $query .= "\n      FROM  after_foil_press_price AS A";

            $query .= "\n     WHERE  A.cate_sortcode = %s";
            $query .= "\n       AND  A.after_name    = %s";
            $query .= "\n       AND  A.dvs           = %s";
            $query .= "\n  ORDER BY  (amt + 0) DESC";
            $query .= "\n     LIMIT  1";

            $query  = sprintf($query, $param["cate_sortcode"]
                , $param["after_name"]
                , $param["dvs"]);

            $rs = $conn->Execute($query);
        }
        $rs_array = array();
        $rs_array["price"] = intval($rs->fields["price"]);
        $rs_array["amt"] = intval($rs->fields["amt"]);
        return $rs_array;
    }

    /**
     * @brief 카테고리 별 회원 할인정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateMemberSaleRate($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.rate";
        $query .= "\n   FROM  cate_member_sale AS A";
        $query .= "\n        ,member           AS B";
        $query .= "\n  WHERE  A.member_seqno  = B.member_seqno";
        $query .= "\n    AND  A.cate_sortcode = %s";
        
        if ($this->blankParameterCheck($param, "member_seqno")) {
            $query .= "\n    AND  A.member_seqno = ";
            $query .= $param["member_seqno"];
        }

        $query  = sprintf($query, $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        $rate = $rs->fields["rate"];

        return doubleval($rate * -1);
    }

    /**
     * @brief 자유형 도무송 가격비율 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterTomsonPricePer($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.amt";
        $query .= "\n           ,A.knife_price_per";
        $query .= "\n           ,A.stick_paper_price_per";
        $query .= "\n           ,A.especial_paper_price_per";
        $query .= "\n           ,A.basic_price";
        $query .= "\n      FROM  after_tomson_price_per AS A";
        $query .= "\n     WHERE  %s <= (A.amt + 0)";
        $query .= "\n  ORDER BY  (A.amt + 0) ASC";
        $query .= "\n     LIMIT  1";

        $query  = sprintf($query, $param["amt"]);

        $rs = $conn->Execute($query);

        // 해당하는 수량이 없을경우 제일 마지막 수량 판매가격 반환
        if ($rs->EOF) {
            $query  = "\n    SELECT  A.amt";
            $query .= "\n           ,A.knife_price_per";
            $query .= "\n           ,A.stick_paper_price_per";
            $query .= "\n           ,A.especial_paper_price_pre";
            $query .= "\n           ,A.basic_price";
            $query .= "\n      FROM  after_tomson_price_per AS A";
            $query .= "\n  ORDER BY  (A.amt + 0) DESC";
            $query .= "\n     LIMIT  1";

            $rs = $conn->Execute($query);
        }

        return $rs->fields;
    }

    /**
     * @brief 자유형 도무송 가격 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAfterTomsonPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("col_name" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.basic_price";
        $query .= "\n        ,A.%s";
        $query .= "\n   FROM  after_tomson_price AS A";
        $query .= "\n  WHERE  A.size_start <= %s";
        $query .= "\n    AND  %s <= A.size_end";

        $query  = sprintf($query, $param["col_name"]
                                , $param["size_start"]
                                , $param["size_end"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 계산형 가격 종이 정보 검색
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 종이 맵핑코드
     * @param $col    = 상품종이에서 검색할 필드
     *
     * @return 종이 기준단위
     */
    function selectPrdtPaperInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mpcode = $param["mpcode"];
        $col    = $param["col"];
        $affil  = $param["affil"];

        $rs = $this->selectCatePaperInfo($conn, $mpcode);

        $search_check = sprintf("%s|%s|%s|%s", $rs["name"]
                                             , $rs["dvs"]
                                             , $rs["color"]
                                             , $rs["basisweight"]);

        unset($temp);

        $temp["col"]   = $col;
        $temp["table"] = "prdt_paper";
        $temp["where"]["affil"]        = $affil;
        $temp["where"]["sort"]         = $rs["sort"];
        $temp["where"]["search_check"] = $search_check;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields;
    }

    /**
     * @brief 계산형 가격 인쇄 정보 검색
     *
     * @param $conn   = connection identifier
     * @param $mpcode = 카테고리 인쇄 맵핑코드
     *
     * @return 인쇄 정보
     */
    function selectPrdtPrintInfo($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mpcode = $this->parameterEscape($conn, $mpcode);

        $query  = "\n    SELECT  A.output_board_amt";
        $query .= "\n           ,A.beforeside_tmpt AS bef_tmpt";
        $query .= "\n           ,A.aftside_tmpt AS aft_tmpt";
        $query .= "\n           ,A.tot_tmpt";
        $query .= "\n           ,B.crtr_unit";
        $query .= "\n           ,B.mpcode AS prdt_mpcode";

        $query .= "\n      FROM  prdt_print      AS A";
        $query .= "\n           ,prdt_print_info AS B";
        $query .= "\n           ,cate_print      AS C";

        $query .= "\n     WHERE  A.print_name       = B.print_name";
        $query .= "\n       AND  A.purp_dvs         = B.purp_dvs";
        $query .= "\n       AND  A.prdt_print_seqno = C.prdt_print_seqno";
        $query .= "\n       AND  C.mpcode           = %s";

        $query  = sprintf($query, $mpcode);

        return $conn->Execute($query)->fields;

    }

    /**
     * @brief 계산형 가격 출력 정보 검색
     *
     * @param $conn   = connection identifer
     * @param $mpcode = 카테고리 규격 맵핑코드
     *
     * @return 검색결과
     */
    function selectPrdtOutputInfo($conn, $mpcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mpcode = $this->parameterEscape($conn, $mpcode);

        $query  = "\n SELECT  A.name";
        $query .= "\n        ,B.mpcode AS prdt_mpcode";

        $query .= "\n   FROM  prdt_stan          AS A";
        $query .= "\n        ,prdt_output_info   AS B";
        $query .= "\n        ,cate_stan          AS C";

        $query .= "\n  WHERE  A.prdt_stan_seqno = C.prdt_stan_seqno";
        $query .= "\n    AND  A.output_name = B.output_name";
        $query .= "\n    AND  A.output_board_dvs = B.output_board_dvs";
        $query .= "\n    AND  C.mpcode = %s";

        $query  = sprintf($query, $mpcode);

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 수량별 종이할인금액 검색
     *
     * @param $conn  = connection identifer
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectAmtPaperSale($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.rate";
        $query .= "\n        ,A.aplc_price";
        $query .= "\n        ,A.singleside_price";

        $query .= "\n   FROM  amt_paper_sale AS A";

        $query .= "\n  WHERE  A.cate_sortcode = %s";
        $query .= "\n    AND  A.cate_paper_mpcode = %s";
        $query .= "\n    AND  A.cate_stan_mpcode  = %s";
        $query .= "\n    AND  A.amt      = %s";
        if ($this->blankParameterCheck($param, "page_amt")) {
            $query .= "\n    AND  A.page_amt = " . $param["page_amt"];
        }
        $query .= "\n    AND  A.page_amt != 0";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["cate_paper_mpcode"]
                                , $param["cate_stan_mpcode"]
                                , $param["amt"]);

        return $conn->Execute($query)->fields;
    }

    /**
     * @brief 제본 가격 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 종이 기준단위
     */
    function selectBindingPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT A.sell_price";
        $query .= "\n      FROM cate_after_price AS A";
        $query .= "\n     WHERE A.cate_after_mpcode = %s";
        $query .= "\n       AND %s <= (A.amt + 0)";
        $query .= "\n  ORDER BY (A.amt + 0) ASC";
        $query .= "\n     LIMIT 1";

        $query  = sprintf($query, $param["mpcode"]
                                , $param["amt"]);

        $rs = $conn->Execute($query);

        // 해당하는 수량이 없을경우 제일 마지막 수량 판매가격 반환
        if ($rs->EOF) {
            $query  = "\n    SELECT A.sell_price";
            $query .= "\n      FROM cate_after_price AS A";
            $query .= "\n     WHERE A.cate_after_mpcode = %s";
            $query .= "\n  ORDER BY (A.amt + 0) DESC";
            $query .= "\n     LIMIT 1";

            $query  = sprintf($query, $param["mpcode"]);

            $rs = $conn->Execute($query);
        }

        return $rs->fields["sell_price"];
    }

    /**
     * @brief 출고일 확인 팝업에서 사용할 주문리스트 검색
     *
     * @detail 상위 10개만 일단 가져옴
     *
     * @param $conn = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateOrderList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  C.member_name";
        $query .= "\n           ,A.depo_finish_date";
        $query .= "\n           ,B.release_date";
        $query .= "\n           ,DATEDIFF(B.release_date, A.depo_finish_date) AS period";
        $query .= "\n      FROM  order_common AS A";
        $query .= "\n           ,order_dlvr   AS B";
        $query .= "\n           ,member       AS C";
        $query .= "\n     WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n       AND  A.member_seqno       = C.member_seqno";
        $query .= "\n       AND  B.tsrs_dvs           = '수신'";
        $query .= "\n       AND  B.release_date IS NOT NULL";
        $query .= "\n       AND  A.cate_sortcode      = %s";
        $query .= "\n     LIMIT  10";

        $query  = sprintf($query, $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 종이 미리보기 정보 가져옴
     *
     * @param $conn = db connection
     *
     * @return 검색결과
     */
    function selectPaperPreviewInfo($conn, $param) {
        //커넥션 체크
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  file_path";
        $query .= "\n       ,save_file_name";
        $query .= "\n       ,origin_file_name";
        $query .= "\n       ,paper_preview_seqno";

        $query .= "\n  FROM  paper_preview";
        $query .= "\n WHERE  1 = 1";

        if ($this->blankParameterCheck($param ,"name")) {
            $query .= "\n   AND  name = $param[name]";
        }
        if ($this->blankParameterCheck($param ,"dvs")) {
            $query .= "\n   AND  dvs = $param[dvs]";
        }
        if ($this->blankParameterCheck($param ,"color")) {
            $query .= "\n   AND  color = $param[color]";
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 카테고리 템플릿 파일정보 검색
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateTemplateFileInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false; 
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  cate_template_seqno";
        $query .= "\n        ,ai_file_path";
        $query .= "\n        ,ai_save_file_name";
        $query .= "\n        ,ai_origin_file_name";
        $query .= "\n        ,eps_file_path";
        $query .= "\n        ,eps_save_file_name";
        $query .= "\n        ,eps_origin_file_name";
        $query .= "\n        ,cdr_file_path";
        $query .= "\n        ,cdr_save_file_name";
        $query .= "\n        ,cdr_origin_file_name";
        $query .= "\n        ,sit_file_path";
        $query .= "\n        ,sit_save_file_name";
        $query .= "\n        ,sit_origin_file_name";

        $query .= "\n   FROM  cate_template";
        $query .= "\n  WHERE  1 = 1";
        $query .= "\n    AND  cate_template_seqno = ";
        $query .= $param["cate_template_seqno"];

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 사이즈 이름, 계열검색
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectSizeNameAffil($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* ProductCommonDAO.inc : 사이즈 이름, 계열검색 */";
        $query .= "\n         A.name";
        $query .= "\n        ,A.affil";
        $query .= "\n   FROM  prdt_stan AS A";
        $query .= "\n        ,cate_stan AS B";
        $query .= "\n  WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n    AND  B.mpcode = %s";

        $query  = sprintf($query, $param["mpcode"]);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 확정형 가격 테이블에서 최소수량 검색
     *
     * @detail 스티커 비규격 검색시 필요한 최소가격 검색용
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMinStan($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n   SELECT  /* ProductCommonDAO.inc - 해당 카테고리 최소사이즈 검색*/";
        $query .= "\n           A.mpcode";
        $query .= "\n          ,B.name";
        $query .= "\n          ,B.typ";
        $query .= "\n     FROM  cate_stan AS A";
        $query .= "\n          ,prdt_stan AS B";
        $query .= "\n    WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n      AND  A.cate_sortcode = %s";
        $query .= "\n GROUP BY  B.typ";
        $query .= "\n ORDER BY  (B.cut_wid_size * B.cut_vert_size) ASC";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"]));

        $bind = [];
        $bind[] = $param["cate_sortcode"];

        $ret = [];
        $rs = $conn->Execute($query, $bind);

        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $ret[$fields["typ"]] = $fields["mpcode"];

            $rs->MoveNext();
        }

        return $ret;
    }

    /**
     * @brief 확정형 가격 테이블에서 최소수량 검색
     *
     * @detail 스티커 비규격 검색시 필요한 최소가격 검색용
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMinAmt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT /* ProductCommonDAO.inc - 해당 카테고리의 최소 수량 검색*/";
        $query .= "\n        MIN(amt) AS min_amt";
        $query .= "\n   FROM ply_price AS A";
        $query .= "\n  WHERE A.cate_sortcode = %s";
        $query .= "\n    AND A.cate_stan_mpcode = %s";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                , $conn->param($param["stan_mpcode"]));

        $bind = [];
        $bind[] = $param["cate_sortcode"];
        $bind[] = $param["stan_mpcode"];

        $rs = $conn->Execute($query, $bind);

        return doubleval($rs->fields["min_amt"]);
    }

    /**
     * @brief 확정형 가격 테이블에서 최소가격 검색
     *
     * @detail 스티커 비규격 검색시 필요한 최소가격 검색용
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMinPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* ProductCommonDAO.inc - 해당 카테고리의 최소 가격 검색*/";
        $query .= "\n         A.new_price";
        $query .= "\n   FROM  ply_price AS A";
        $query .= "\n  WHERE  A.cate_sortcode = %s";
        $query .= "\n    AND  A.cate_stan_mpcode = %s";
        $query .= "\n    AND  A.cate_paper_mpcode = %s";
        $query .= "\n    AND  A.amt = %s";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                , $conn->param($param["stan_mpcode"])
                                , $conn->param($param["paper_mpcode"])
                                , $conn->param($param["amt"]));
        $bind = [];
        $bind[] = $param["cate_sortcode"];
        $bind[] = $param["stan_mpcode"];
        $bind[] = $param["paper_mpcode"];
        $bind[] = $param["amt"];
        $rs = $conn->Execute($query, $bind);

        return intval($rs->fields["new_price"]);
    }

    /**
     * @brief 카테고리 옵션 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 카테고리 분류코드, 상품구분값
     *
     * @return ul html
     */
    function selectCateAoOptHtml($conn, $param, $def_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_sortcode = $param["cate_sortcode"];
        $cate_sortcode = $this->parameterEscape($conn, $cate_sortcode);

        $query  = "\n SELECT  A.opt_name AS name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,B.mpcode";
        $query .= "\n        ,B.basic_yn";
        $query .= "\n        ,B.seq";

        $query .= "\n   FROM  ao_opt AS A";
        $query .= "\n        ,cate_ao_opt AS B";

        $query .= "\n  WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n  ORDER  BY seq ASC";

        $query  = sprintf($query, $cate_sortcode);

        $rs = $conn->Execute($query);

        $info_arr = [];
        $info_arr["dvs"] = $param["dvs"];
        $info_arr["is_ao"] = true;
        $html = makeCateOptUl($rs, $info_arr, $def_arr, $cate_sortcode, $param['link']);
        unset($info_arr["dvs"]);
        unset($info_arr["is_ao"]);

        return ["info_arr" => $info_arr,
                "html"     => $html];
    }

    /**
     * @brief 카테고리 추가 옵션 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return option html
     */
    function selectCateAddAoOptInfoHtml($conn, $param, $def_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mobile_yn = $param["mobile_yn"];
        $cate_sortcode = $this->parameterEscape($conn, $param["cate_sortcode"]);

        $query  = "\n SELECT  A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  ao_opt AS A";
        $query .= "\n        ,cate_ao_opt AS B";

        $query .= "\n  WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        $query .= "\n    AND  B.basic_yn = 'N'";
        $query .= "\n    AND  A.opt_name IN (%s)";

        $query  = sprintf($query, $cate_sortcode, $param["opt_name"]);

        $rs = $conn->Execute($query);

        return makeCateAddOpt($rs,
                              $param["opt_idx"],
                              $param["dvs"],
                              $mobile_yn,
                              true,
                              $def_arr);
    }

    /**
     * @brief 카테고리 후공정 검색
     *
     * @param $conn          = connection identifier
     * @param $param         = 카테고리 분류코드, 제품 구분값
     * @param $except_arr    = 검색제외배열
     *
     * @return ul html
     */
    function selectCateAoAfterHtml($conn, $param, $def_arr = [], $except_arr = []) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $cate_sortcode = $this->parameterEscape($conn, $param["cate_sortcode"]);
        $dvs = $param["dvs"];

        $query  = "\n SELECT  A.after_name AS name";
        $query .= "\n        ,B.basic_yn";

        $query .= "\n   FROM  ao_after AS A";
        $query .= "\n        ,cate_ao_after AS B";

        $query .= "\n  WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";

        $query  = sprintf($query, $cate_sortcode);

        $rs = $conn->Execute($query);

        $info_arr = [];
        $info_arr["is_ao"] = true;
        $html = makeCateAftUl($rs, $dvs, $info_arr, $def_arr, $except_arr);
        unset($info_arr["is_ao"]);

        return ["info_arr" => $info_arr,
                "html"     => $html];
    }

    /**
     * @brief 실사출력 카테고리 추가 후공정 검색
     *
     * @param $conn          = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return option html
     */
    function selectCateAddAoAfterInfoHtml($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $mobile_yn = $param["mobile_yn"];
        $dvs = $param["dvs"];

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.after_name";
        $query .= "\n           ,A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,B.mpcode";

        $query .= "\n      FROM  ao_after AS A";
        $query .= "\n           ,cate_ao_after AS B";

        $query .= "\n     WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  B.basic_yn = 'N'";
        $query .= "\n       AND  A.after_name = %s";

        $query .= "\n  ORDER BY  B.seq";
        $query .= "\n           ,A.after_name";
        $query .= "\n           ,A.ao_after_seqno";

        $query  = sprintf($query, $param["cate_sortcode"]
                                , $param["after_name"]);

        $rs = $conn->Execute($query);

        return makeCateAddAoAfter($rs, $dvs, $mobile_yn);
    }

    /**
     * @brief 기간 내에 해당하는 휴일 개수 검색
     *
     * @param $conn = connection identifier
     * @param $from = 시작일
     * @param $to   = 종료일
     *
     * @return 휴일 개수
     */
    function selectHoliInfoCount($conn, $from, $to) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT COUNT(1) AS cnt";
        $query .= "\n   FROM holi_info AS A";
        $query .= "\n  WHERE A.date BETWEEN %s AND %s";

        $query  = sprintf($query, $conn->param($from)
                                , $conn->param($to));

        $temp = [];
        $temp[] = $from;
        $temp[] = $to;

        $rs = $conn->Execute($query, $temp);

        return intval($rs->fields["cnt"]);
    }

    /**
     * @brief 선택된 옵션 불러오기
     */
    function selectOrderOptions($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT A.*, B.*, ";
        $query .= "\n (select group_concat(CONCAT(after_name) SEPARATOR '|') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') as after_name, ";
        $query .= "\n (SELECT GROUP_CONCAT(CONCAT(info) SEPARATOR ',') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') AS after_info,";
        $query .= "\n (SELECT GROUP_CONCAT(CONCAT(depth1) SEPARATOR ',') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') AS after_depth1,";
        $query .= "\n (SELECT GROUP_CONCAT(CONCAT(depth2) SEPARATOR ',') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') AS after_depth2,";
        $query .= "\n (SELECT GROUP_CONCAT(CONCAT(depth3) SEPARATOR ',') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') AS after_depth3, ";
        $query .= "\n (SELECT GROUP_CONCAT(CONCAT(mpcode) SEPARATOR ',') FROM order_after_history WHERE order_common_seqno = %s AND basic_yn = 'N') AS mpcode";
        $query .= "\n   FROM order_common AS A ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n  WHERE  A.order_common_seqno = %s ";

        $query  = sprintf($query,
            $param['order_common_seqno'],
            $param['order_common_seqno'],
            $param['order_common_seqno'],
            $param['order_common_seqno'],
            $param['order_common_seqno'],
            $param['order_common_seqno'],
            $param['order_common_seqno']);
        return $conn->Execute($query);
    }
}
?>
