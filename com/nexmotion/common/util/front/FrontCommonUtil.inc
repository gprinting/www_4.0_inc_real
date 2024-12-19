<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . '/define/front/common_config.inc');
include_once(INC_PATH . '/common_lib/CommonUtil.inc');
include_once(INC_PATH . "/common_define/prdt_default_info.inc");
include_once(INC_PATH . "/common_define/cscenter_define.inc");

class FrontCommonUtil extends CommonUtil {
    /**
     * @brief 카테고리 대중소 분류코드 반환
     *
     * @detail 카테고리 중, 소 분류코드가 무조건 001로 시작한다고
     * 장담할 수 없으므로 DB검색으로 가장 작은값을 먼저 가져옴
     *
     * @param $conn = connection identifier
     * @param $dao  = 카테고리 분류코드 검색용 dao
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return array(
     *             "sortcode_t" => 카테고리 대분류 분류코드,
     *             "sortcode_m" => 카테고리 중분류 분류코드,
     *             "sortcode_b" => 카테고리 소분류 분류코드
     *         )
     */
    function getTMBCateSortcode($conn, $dao, $cate_sortcode) {
        $sortcode_t = false;
        $sortcode_m = false;
        $sortcode_b = false;

        if (strlen($cate_sortcode) === 3) {
            $sortcode_t = $cate_sortcode;
            $sortcode_m = $dao->selectCateSortcode($conn, $sortcode_t);
            $sortcode_b = $dao->selectCateSortcode($conn, $sortcode_m);
        }
        if (strlen($cate_sortcode) === 6) {
            $sortcode_t = substr($cate_sortcode, 0, 3);
            $sortcode_m = $cate_sortcode;
            $sortcode_b = $dao->selectCateSortcode($conn, $sortcode_m);
        }
        if (strlen($cate_sortcode) === 9) {
            $sortcode_t = substr($cate_sortcode, 0, 3);
            $sortcode_m = substr($cate_sortcode, 0, 6);
            $sortcode_b = $cate_sortcode;
        }

        return array("sortcode_t" => $sortcode_t,
                     "sortcode_m" => $sortcode_m,
                     "sortcode_b" => $sortcode_b);
    }

    /**
     * @brief 옵션/후공정 depth1, 2, 3을 붙여서 하나로 만듬
     *
     * @detail 추가 옵션/후공정 팝업에서 사용
     *
     * @param $info  = 옵션/후공정 정보
     *
     * @return 옵션/후공정 풀네임
     */
    function getOptAfterFullName($info) {
        $depth1 = $info["depth1"];
        $depth2 = $info["depth2"];
        $depth3 = $info["depth3"];

        $dvs = '';

        if ($depth1 !== '-') {
            $dvs = $depth1;
        }
        if ($depth2 !== '-') {
            $dvs .= ' ' . $depth2;
        }
        if ($depth3 !== '-') {
            $dvs .= ' ' . $depth3;
        }

        return $dvs;
    }

    /**
     * @brief 사이드 메뉴 주문상태 요약 배열 생성
     *
     * @detail $ret["200"] = 입금대기
     * $ret["300"] = 접수
     * $ret["400"] = 조판
     * $ret["600"] = 출력
     * $ret["700"] = 인쇄
     * $ret["800"] = 후공정
     * $ret["900"] = 입고
     * $ret["950"] = 출고
     * $ret["010"] = 배송 
     * $ret["020"] = 완료
     *
     * @param $rs = 검색결과
     *
     * @return 주문상태 + 진행상태값
     */
    function makeOrderSummaryArr($rs) {
        $ret = array(
            "입금대기" => 0,
            "접수"     => 0,
            "제작"     => 0,
            "입출고"   => 0,
            "배송"     => 0,
            "완료"     => 0
        );

        while ($rs && !$rs->EOF) {
            $state_name  = $rs->fields["dvs"];
            $state_count = intval($rs->fields["state_count"]);

            switch ($state_name) {
            // 입금대기
            case "입금" :
                $ret["입금대기"] += $state_count;
                break;
            // 접수
            case "접수" :
                $ret["접수"] += $state_count;
                break;
            // 제작
            case "조판" :
            case "출력" :
            case "인쇄" :
            case "조판후공정" :
            case "주문후공정" :
                $ret["제작"] += $state_count;
                break;
            // 입출고
            case "입고" :
            case "출고" :
                $ret["입출고"] += $state_count;
                break;
            // 배송
            case "배송" :
                $ret["배송"] += $state_count;
                break;
            // 완료
            case "구매확정" :
                $ret["완료"] += $state_count;
                break;
            }

            $rs->MoveNext();
        }

        return $ret;
    }

    /**
     * @brief 잘못 된 접근시
     *
     * @param $title = alert 내용
     *
     * @return 계산된 값
     */
    function errorGoBack($title = "") {
        echo "<script>";
        echo "    alert('" . $title . "');";
        echo "    history.go(-1);";
        echo "</script>";
    }

    /**
     * @brief 종이 정보 문자열 생성
     * 구분값이 '-' 일 경우 빼고 생성
     *
     * @param $arr = 종이 정보 배열
     * @param $flag_arr = 표시여부 배열
     *
     * @return 종이 정보 문자열
     */
    function makePaperInfoStr($arr, $flag_arr) {
        if (empty($flag_arr)) {
            $flag_arr = [
                 "name"        => true
                ,"dvs"         => true
                ,"color"       => true
                ,"basisweight" => true
            ];
        }

        $name        = $arr["name"];
        $dvs         = $arr["dvs"];
        $color       = $arr["color"];
        $basisweight = $arr["basisweight"];

        $ret = '';
        if (!empty($name) && $flag_arr["name"]) {
            $ret = $name;
        }
        if ($dvs !== '-' && $flag_arr["dvs"]) {
            if (!empty($ret)) {
                $ret .= ' ';
            }
            $ret .= $dvs;
        }
        if ($color !== '-' && $flag_arr["color"]) {
            if (!empty($ret)) {
                $ret .= ' ';
            }
            $ret .= $color;
        }
        if ($basisweight !== "0g" && $flag_arr["basisweight"]) {
            if (!empty($ret)) {
                $ret .= ' ';
            }
            $ret .= $basisweight;
        }

        return trim($ret);
    }

    function makeSingleInfoStr($arr, $flag_arr) {
        if (empty($flag_arr)) {
            $flag_arr = [
                "name"        => true
                ,"dvs"         => true
                ,"color"       => true
                ,"basisweight" => true
            ];
        }

        $name        = $arr["name"];
        $dvs         = $arr["dvs"];
        $color       = $arr["color"];
        $basisweight = $arr["basisweight"];

        $ret = '';
        if (!empty($name) && $flag_arr["name"]) {
            $ret = $name;
        }

        return trim($ret);
    }

    /**
     * @brief 문자열 길이 자르기
     *
     * @param $str = 원본문자열
     * @param $start = 추출할 문자열 시작 위치
     * @param $end = 추출할 문자열 끝 위치
     * @param $tail = 뒤에 붙일 문자열
     *
     * @return 다듬어진 문자열
     */
    function str_cut($str, $start = 0, $end, $tail = "..") {
    
        if (!$str) return "";

        if (strlen($str) > $end)
            return mb_substr($str, $start, $end, 'UTF-8') . $tail;
        else 
            return $str;
    }

    /**
     * @brief 묶음배송용 식별값 생성
     *
     * @return 식별값
     */
    function makeBunDlvrOrderNum() {
        $t = strval(ceil(microtime(true) * 1000.0));
        $r = rand(0, 9);

        return $t . $r;
    }

    /**
     * @brief 주문번호 생성
     *
     * @detail 주문번호 양식은 다음과 같다
     * 채널(3)날짜(YYMMDD)품목(2)일련번호(5)
     * GPT160921NC00001
     *
     * @param $conn  = db connection
     * @param $dao   = dao 객체
     * @param $param = 검색용 파라미터
     *
     * @return 주문번호
     */
    function makeOrderNum($conn, $dao, $cate_sortcode) {
        //$sell_site     = $dao->selectCpnAdminNick($conn, $_SESSION["sell_site"]);
        $sell_site     = $_SERVER["SELL_SITE"] . "T";

        if($sell_site == "T") $sell_site = "GPT";
        $cate_sortcode = substr($cate_sortcode, 0, 3);
        $cate_nick     = $dao->selectCateNick($conn, $cate_sortcode);
        $last_num      = $dao->selectOrderCommonLastNum($conn);
        $last_num      = str_pad($last_num, 5, '0', STR_PAD_LEFT);

        $form = "%s%s%s";
        $form = sprintf($form, $sell_site
                             , date("ymd")
                             , $cate_nick);
        return $form;
    }

    function makeBizhowsOrderNum($conn, $dao, $cate_sortcode) {
        //$sell_site     = $dao->selectCpnAdminNick($conn, $_SESSION["sell_site"]);
        $sell_site     = $_SERVER["SELL_SITE"] . "T";
        if($sell_site == "T") $sell_site = "GPT";
        $cate_sortcode = substr($cate_sortcode, 0, 3);
        $cate_nick     = $dao->selectCateNick($conn, $cate_sortcode);
        $last_num      = $dao->selectOrderCommonLastNum($conn);
        $last_num      = str_pad($last_num, 5, '0', STR_PAD_LEFT);

        $form = "%s%s%s";
        $form = sprintf($form, $sell_site
            , date("ymd")
            , $cate_nick);
        return $form;
    }

    /**
     * @brief 주문별 원파일 그룹번호 생성
     *
     * @param $param = 검색용 파라미터
     *
     * @return 그룹번호
     */
    function makeOnefileGroupNum($member_seqno) {
        return str_pad(dechex(crc32($member_seqno)), 8, '0', STR_PAD_LEFT)
               . '_' .  date("ymdHis");
    }

    /**
     * @brief 예상무게 계산
     *
     * @param $conn  = connection identifier
     * @param $dao   = 정보검색을 수행할 dao
     * @param $param = 제품 구분 접두어, 폼빈, 유틸
     *
     * @return 예상무게값
     */
    function calcExpectWeight($conn, $dao, $param) {
        $prefix = $param["prefix"];
        $fb     = $param["fb"];
        //$util   = $param["util"];

        $divide_val = 1000000000.0;

        $cate_sortcode = $fb[$prefix . "cate_sortcode"];
        $sortcode_t = substr($cate_sortcode, 0, 3);
        $sortcode_m = substr($cate_sortcode, 0, 6);

        if (empty($fb[$prefix . "sheet_count"])) {
            $amt = doubleval($fb[$prefix . "amt"]);
        } else {
            $amt = doubleval($fb[$prefix . "sheet_count"]);
        }

        if ($amt < 0.0) {
            return 0.0;
        }

        if (intval($fb[$prefix . "page"]) < 0) {
            return 0.0;
        }

        // ncr용 변수
        $basisweight_arr = explode('|', $fb[$prefix . "basisweight"]);

        if ($sortcode_m === "001001") {
            // 책자일 때 수량계산
            $temp = [
                "amt"       => $fb[$prefix . "amt"],
                "pos_num"   => $fb[$prefix . "pos_num"],
                "page_num"  => $fb[$prefix . "page"],
                "amt_unit"  => $fb[$prefix . "amt_unit"],
                "crtr_unit" => ''
            ];
            $amt = $this->getPaperRealPrintAmt($temp);

        } else if ($sortcode_t === "006") {
            $amt = $amt * 2;
        } else if ($sortcode_m === "007001") {
            // 마스터 NCR
            $amt = $amt * PrdtDefaultInfo::MST_GROUP;
            // 곱하기 상중/상중하/상중중하 2/3/4 곱하기 필요

            $temp = [
                "amt"       => $amt * count($basisweight_arr),
                "pos_num"   => $fb[$prefix . "pos_num"],
                "page_num"  => '2',
                "amt_unit"  => $fb[$prefix . "amt_unit"],
                "crtr_unit" => ''
            ];
            $amt = $this->getPaperRealPrintAmt($temp);

        } else if ($sortcode_m === "007002") {
            // 마스터 양식지
            $amt = $amt * 2 * PrdtDefaultInfo::MST_GROUP;

            $temp = [
                "amt"       => $amt,
                "pos_num"   => $fb[$prefix . "pos_num"],
                "page_num"  => '2',
                "amt_unit"  => $fb[$prefix . "amt_unit"],
                "crtr_unit" => ''
            ];
            $amt = $this->getPaperRealPrintAmt($temp);
        }

        $count = intval($fb[$prefix . "count"]);
        if ($count === 0) {
            $count = 1;
        }

        $paper_mpcode = $fb[$prefix . "paper"];

        if (intval($paper_mpcode) < 0) {
            return 0.0;
        }

        $ret = 0;
        $basisweight = 0;

        $cut_size_wid  = intval($fb[$prefix . "cut_wid_size"]);
        $cut_size_vert = intval($fb[$prefix . "cut_vert_size"]);

        if ($sortcode_m === "007001") {
            // 마스터 NCR
            $ret = 0.0;

            foreach ($basisweight_arr as $basisweight) {
                $basisweight = substr($basisweight, 0, -1);
                $basisweight = intval($basisweight);

                $temp  = doubleval($basisweight *
                                   $amt *
                                   $cut_size_wid *
                                   $cut_size_vert *
                                   $count);
                $temp /=  $divide_val;
                $ret += $temp;
            }
        } else if ($sortcode_t=== "004") {
            // 스티커
            $basisweight = 190;

            $ret  = doubleval($basisweight *
                              $amt *
                              $cut_size_wid *
                              $cut_size_vert *
                              $count);
            $ret /=  $divide_val;
        } else if ($cate_sortcode == "012007001") {
            $ret = 12.4 * $count;
        } else if ($cate_sortcode == "012007002") {
            $ret = 12.9 * $count;
        } else {
            // 나머지
            $basisweight = $dao->selectCatePaperInfo($conn, $paper_mpcode);
            $basisweight = substr($basisweight["basisweight"] , 0, -1);
            $basisweight = intval($basisweight);

            $ret  = doubleval($basisweight * $amt *
                              $cut_size_wid * $cut_size_vert *
                              $count);
            $ret /= $divide_val;
        }

        $ret = floor($ret * 10.0) / 10.0;

        if ($ret === 0.0) {
            $ret = 0.1;
        }

        return $ret;
    }

    /**
     * @brief 당일판 depth1과 현 시간에 따라 당일판이 가능한지 체크
     *
     * @param $depth1 = 당일판 depth1(오후12시 마감 / 오후3시 마감)
     *
     * @return 당일판 가능하면 true / 아니면 false
     */
    function chkAvailEmergency($depth1) {
        $time = intval(date("His"));

        if ($depth1 === "오후12시 마감") {
            if (120000 < $time && $time < 200000) {
                return false;
            }
        } else if ($depth1 === "오후3시 마감") {
            if (120000 <= $time && $time < 190000) {
                return false;
            }
        } else if ($depth1 === "오후1시30분 마감") {
            if (133000 <= $time && $time < 190000) {
                return false;
            }
        } else if ($depth1 === "오후2시30분 마감") {
            if (143000 <= $time && $time < 190000) {
                return false;
            }
        } else if ($depth1 === "오후2시 마감") {
            if (140000 <= $time && $time < 190000) {
                return false;
            }
        }



        return true;
    }

    /**
      */
    function getLeafletBoxcount($expec_weight) {
        $count = 1;

        if ($expec_weight > 32) {
            $count = (int)($expec_weight / 25) + 1;
        }

        return $count;
    }

    function getLumpCount($sortcode, $weight) {
        $base_dlvr_price = 2750;

        $sortcode_t = substr($sortcode ,0, 3);
        $sortcode_m = substr($sortcode ,0, 6);
        if ($sortcode === "008001001"
                || $sortcode_m === "008002"
                || $sortcode_t === "003"
                || $sortcode_t === "004") {
            // 명함류, 한 덩어리당 12kg
            $box_count = (int)($weight / 12) + 1;
        } else {
            $box_count = 1;

            // 전단류, 한 덩어리당 32kg, 넘어가면 25kg 단위로 자름
            if($weight > 25) {
                $box_count = $this->getLeafletBoxcount($weight);
            }
        
            $base_dlvr_price = 4400;
        }
        
        $price = $base_dlvr_price * $box_count;

        $ret = ["lump_count" => $box_count
               ,"price"      => $price];

        return $ret;
    }

    /**
     * @brief 카테고리 소분류에 따른 가이드 파일명 매칭
     *
     * @param $cs = 카테고리소분류 sortcode
     *
     * @return 가이드 파일명
     */
    function selectWorkGuideFileName($cs) {
    
        $file_name = WORK_GUIDE_ARR[$cs]; 

        return $file_name;
    }

     /**
     * @brief 현수막 갯수에 따른 배송비 기본단가
     *
     * @param $num = 갯수
     *
     * @return 
     */
    function bannerBasicPrice($num) {

        /*
            1~5	7000
6~10	14000
11~15	21000
16~20	28000
21~25	35000
30~50	42000
55	77000
60	84000
65	91000
70	98000
75	105000
80	112000
85	119000
90	126000
95	133000
100	140000
110	154000
120	168000
130	182000
140	196000
150	210000
160	224000
170	238000
180	252000
190	266000
200	280000
        */

        if($num <= 5){
            $bprice = 7000;
        }else if(5 < $num && $num < 11){
             $bprice = 14000;
        }else if(10 < $num && $num < 16){
             $bprice = 21000;
        }else if(15 < $num && $num < 21){
             $bprice = 28000;           
        }else if(20 < $num && $num < 26){
             $bprice = 35000;            
            }else if(29 < $num && $num < 51){
             $bprice = 42000;           
        }else if($num == 55){
             $bprice = 77000;
        }else if($num == 60){
             $bprice = 84000;
        }else if($num == 65){
             $bprice = 91000;
        }else if($num == 70){
             $bprice = 98000;
        }else if($num == 75){
             $bprice = 105000;
        }else if($num == 80){
             $bprice = 123200;
        }else if($num == 85){
             $bprice = 119000;
        }else if($num == 90){
             $bprice = 126000;
        }else if($num == 95){
             $bprice = 133000;
        }else if($num == 100){
             $bprice = 140000;
        }else if($num == 110){
             $bprice = 154000;
        }else if($num == 120){
             $bprice = 168000;
        }else if($num == 130){
             $bprice = 182000;
        }else if($num == 140){
             $bprice = 196000;
        }else if($num == 150){
             $bprice = 210000;
        }else if($num == 160){
             $bprice = 224000;
        }else if($num == 170){
             $bprice = 238000;
        }else if($num == 180){
             $bprice = 252000;
        }else if($num == 190){
             $bprice = 266000;
        }else if($num == 200){
             $bprice = 280000;
        }else{
             $bprice = 280000;
        }

    

        return $bprice;
    }

    function bannerBasicbox($num) {

        if($num <= 5){
            $box = 1;
        }else if(5 < $num && $num < 11){
             $box = 2;
        }else if(10 < $num && $num < 16){
             $box = 3;
        }else if(15 < $num && $num < 21){
             $box = 4;           
        }else if(20 < $num && $num < 26){
             $box = 5;            
        }else if($num == 30){
             $box = 6;           
        }else if($num == 35){
             $box = 7;           
        }else if($num == 40){
             $box = 8;           
        }else if($num == 45){
             $box = 9;           
        }else if($num == 50){
             $box = 10;           
        }else if($num == 55){
             $box = 11;
        }else if($num == 60){
             $box = 12;
        }else if($num == 65){
             $box = 13;
        }else if($num == 70){
             $box = 14;
        }else if($num == 75){
             $box = 15;
        }else if($num == 80){
             $box = 16;
        }else if($num == 85){
             $box = 17;
        }else if($num == 90){
             $box = 18;
        }else if($num == 95){
             $box = 19;
        }else if($num == 100){
             $box = 20;
        }else if($num == 110){
             $box = 22;
        }else if($num == 120){
             $box = 24;
        }else if($num == 130){
             $box = 26 ;
        }else if($num == 140){
             $box = 28;
        }else if($num == 150){
             $box = 30;
        }else if($num == 160){
             $box = 32;
        }else if($num == 170){
             $box = 34;
        }else if($num == 180){
             $box = 36;
        }else if($num == 190){
             $box = 38;
        }else if($num == 200){
             $box = 40;
        }else{
            $box = 40;
        }

    

        return $box;
    }

}
?>
