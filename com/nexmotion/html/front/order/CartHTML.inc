<?
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/job/front/mypage/OrderInfoDAO.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");
/**
 * @param 장바구니 리스트 html 생성
 *
 * @param $conn = connection identifier
 * @param $dao  = 후공정, 사진경로 검색용
 * @param $rs   = 검색결과
 * @param $price_info_arr = 가격정보배열
 *
 * return 장바구니 리스트 html
 */
function makeCartOrderListHtml($conn, $dao, $rs, &$price_info_arr) {
    $ret = "";

    $upper_tr  = "<tr>";
    $upper_tr .= "    <td><label class=\"check\"><input type=\"checkbox\" name=\"seq\" class=\"_individual\" value=\"%s\" onclick=\"calcCartListPriceBox();\" checked><i><i></i></i></label></td>"; //#0 주문공통일련번호
    $upper_tr .= "    <td>%s</td>"; //#1 번호
    $upper_tr .= "    <td>%s</td>"; //#2 담은날
    $upper_tr .= "    <td class=\"left\" id=\"title_td_%s\" style=\"text-align:left;\">";
    $upper_tr .= "        <dl class=\"orderDetail\">";
    $upper_tr .= "            <dt><span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span></dt>"; // #3 td id, 카테고리, 제목
    $upper_tr .= "            <dd>";
    $upper_tr .= "                %s"; //#4 상품정보
    $upper_tr .= "            </dd>";
    $upper_tr .= "        </dl>";
    $upper_tr .= "    </td>";
    $upper_tr .= "    <td class=\"left\">";
    $upper_tr .= "        <ul class=\"post\">";
    $upper_tr .= "            %s"; //#F 후공정정보
    $upper_tr .= "        </ul>";
    $upper_tr .= "    </td>";
    $upper_tr .= "    <td class=\"amount\">%s%s(%s건)</td>"; //#5 수량
    $upper_tr .= "    <td class=\"price\">%s원</td>"; //#6 가격
    $upper_tr .= "    <input type=\"hidden\" id=\"sell_price_%s\" name=\"sell_price\" value=\"%s\">"; //#6-1 정상판매가
    $upper_tr .= "    <input type=\"hidden\" id=\"grade_sale_price_%s\" name=\"grade_sale_price\" value=\"%s\">"; //#6-2 회원등급할인가
    $upper_tr .= "    <input type=\"hidden\" id=\"member_sale_price_%s\" name=\"member_sale_price\" value=\"%s\">"; //#6-3 회원특별할인가
    $upper_tr .= "    <input type=\"hidden\" id=\"event_price_%s\" name=\"event_price\" value=\"%s\">"; //#6-4 이벤트할인가
    $upper_tr .= "    <input type=\"hidden\" id=\"add_after_price_%s\" name=\"add_after_price\" value=\"%s\">"; //#6-5 후공정 가격추가
    $upper_tr .= "    <input type=\"hidden\" id=\"add_opt_price_%s\" name=\"add_opt_price\" value=\"%s\">"; //#6-6 옵션 가격추가
    $upper_tr .= "    <input type=\"hidden\" id=\"is_upload_%s\" name=\"is_upload\" value=\"%s\">"; //#6-7 파일 올라갔는지 확인
    //$upper_tr .= "    <td>";
    $upper_tr .= "    <td class=\"btn menu\">";
    //$upper_tr .= "        <button class=\"%s upload _showOrderDetails _on\" onclick=\"toggleFileBox('%s', '%s', '%s', this, %s);\" title=\"파일 업로드\">파일 업로드</button>"; // #7 인덱스, file_seqno, 파일업로드 yn 이미지, 장바구니 상태값
    //$upper_tr .= "        <button class=\"menuClose\" onclick=\"closeButtonBox('%s');\" title=\"메뉴 접기\">메뉴 접기</button>";  #8 인덱스
    $upper_tr .= "        <div class=\"menuWrap _slide_%d\">"; // #10 버튼모음
    $upper_tr .= "            <ul class=\"ul_fileUpload\">";
    $upper_tr .= "                 <li>";
    $upper_tr .= "                     <button class=\"reUpload\" onclick=\"showReUploadPop('%s', '%s', '%s', '%s','%s');\">"; // #10-1 파일 재업로드
    $upper_tr .= "                        파일 재전송";
    $upper_tr .= "                     </button>";
    $upper_tr .= "                 </li>";
    $upper_tr .= "            </ul>";
    $upper_tr .= "        </div>";
    $upper_tr .= "    </td>";
    $upper_tr .= "</tr>";

    $sum_sell_price       = 0;
    $sum_grade_sale_price = 0;
    $sum_event_price      = 0;

    $i = 1;
    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $cate_sortcode = $fields["cate_sortcode"];

        $order_common_seqno = $fields["order_common_seqno"];
        $order_regi_date    = $fields["order_regi_date"];
        $title              = $fields["title"];
        $order_detail       = $fields["order_detail"];
        $sell_price         = doubleval($fields["sell_price"]);
        $ori_sell_price     = doubleval($fields["sell_price"]);
        $grade_sale_price   = doubleval($fields["grade_sale_price"]);
        $member_sale_price  = doubleval($fields["member_sale_price"]);
        $add_opt_price      = doubleval($fields["add_opt_price"]);
        $add_after_price    = doubleval($fields["add_after_price"]);
        $event_price        = doubleval($fields["event_price"]);
        $expec_weight       = $fields["expec_weight"];
        $amt                = $fields["amt"];
        $amt_unit_dvs       = $fields["amt_unit_dvs"];
        $count              = $fields["count"];

        if (empty($expec_weight) === true || $expec_weight === '0') {
            $expec_weight = '-';
        } else {
            $expec_weight .= "Kg";
        }

        // 가격합산 정보 생성
        $sum_sell_price        += $sell_price;
        $sum_grade_sale_price  += $grade_sale_price * -1;
        $sum_member_sale_price += $member_sale_price * -1;
        $sum_event_price       += $event_price;
        $sum_add_after_price   += $add_after_price;
        $sum_add_opt_price     += $add_opt_price;

        $sell_price += $grade_sale_price;
        $sell_price += $member_sale_price;
        $sell_price += $add_after_price;
        $sell_price += $add_opt_price;

        // 파일 업로드 여부에 따라 버튼 변경
        $file_image = "menu";
        $file_upload = $fields["origin_file_name"];

        if (empty($fields["order_file_seqno"])) {
            $file_image = "required";
            $file_upload = '0';
        }
        // 전체 html 생성
        $ret .= sprintf($upper_tr, $order_common_seqno               //#0
            , $i                                //#1
            , explode(' ', $order_regi_date)[0] //#2
            , $order_common_seqno               //#3
            , trim(explode(',', (explode('/', $order_detail)[0]))[0]) // #3 카테고리
            , $title                            //#3 제목
            , explode('/', $order_detail, 2)[1] //#4 세부정보
            , makeAfterHtml($conn, $order_common_seqno, '') //#F 후공정
            , $amt                              //#5 수량
            , $amt_unit_dvs                     //#5 수량
            , $count                            //#5 수량
            , number_format($sell_price)        //#6 가격
            , $order_common_seqno               //#6-1 정상판매가 seqno
            , number_format($ori_sell_price)    //#6-1 정상판매가
            , $order_common_seqno               //#6-2 회원등급 seqno
            , number_format($grade_sale_price)  //#6-2 회원등급할인가
            , $order_common_seqno               //#6-3 회원특별 seqno
            , number_format($memeber_sale_price) //#6-3 회원특별할인가
            , $order_common_seqno               //#6-4 이벤트 seqno
            , number_format($event_price)       //#6-4 이벤트가격
            , $order_common_seqno               //#6-5 후공정 seqno
            , number_format($add_after_price)   //#6-5 후공정 추가가격
            , $order_common_seqno               //#6-6 옵션 seqno
            , number_format($add_opt_price)     //#6-6 옵션 추가가격
            , $order_common_seqno               //#6-7
            , $file_upload                      //#6-7
            , $file_image                       //#7 버튼열기
            , $i                                //#10 버튼모음
            , $price_info_arr["state"]          //#10-1
            , $order_common_seqno               //#10-1
            , $fields["file_path"] . $fields["save_file_name"]       //#10-1
            , $fields["origin_file_name"]       //#10-1
            , $fields["file_path"] . $fields["save_file_name"]       //#10-1
        );
        $i++;

        $rs->MoveNext();
    }

    $price_info_arr["sell"]  = $sum_sell_price +
        $sum_add_after_price +
        $sum_add_opt_price;
    $price_info_arr["grade"] = $sum_grade_sale_price;
    $price_info_arr["member"] = $sum_member_sale_price;
    //$price_info_arr["event"] = $sum_event_price;
    $price_info_arr["sum"] = $sum_sell_price +
        $sum_add_after_price +
        $sum_add_opt_price -
        $sum_grade_sale_price -
        $sum_member_sale_price -
        $sum_event_price;

    return $ret;
}

/**
 * @param 장바구니 리스트 모바일 html 생성
 *
 * @param $conn = connection identifier
 * @param $dao  = 후공정, 사진경로 검색용
 * @param $rs   = 검색결과
 * @param $price_info_arr = 가격정보배열
 *
 * return 장바구니 리스트 html
 */
function makeMobileCartOrderListHtml($conn, $dao, $rs, &$price_info_arr) {
    $ret = "";

    $upper_tr  = "<tr>";
    $upper_tr .= "    <td><span class=\"after_option_span\" style=\"text-align:right;\"><input type=\"checkbox\" name=\"seq\" id=\"chk_list_%s\" value=\"%s\"><label for=\"chk_list_%s\">&nbsp;</label></span></td>"; //#0 주문공통일련번호
    $upper_tr .= "    <td id=\"title_td_%s\">%s</td>"; //#1 인쇄물제목
    $upper_tr .= "    <td>%s 원</td>"; //#2 가격
    $upper_tr .= "    <td>";
    $upper_tr .= "        <div class=\"btn_board_list_manage\" onclick=\"openLayerPopup('%s')\">"; //#3 파일업로드 버튼
    $upper_tr .= "            <img src=\"/design_template/m/images/common/btn_file.png\" alt=\"파일업로드\">";
    $upper_tr .= "        </div>";
    $upper_tr .= "\n      <div id=\"det_op_%d\" class=\"btn_board_list_manage cart_btn_div\" onclick=\"openOrderDetail('%s', '%s', '4');\">";
    $upper_tr .= "            <img src=\"/design_template/m/images/common/btn_list_detail_on.png\" alt=\"▼\">"; //#7 상세열기
    $upper_tr .= "        </div>";
    $upper_tr .= "\n      <div id=\"det_cl_%d\" class=\"btn_board_list_manage\" onclick=\"closeOrderDetail('%s');\" style=\"display:none;\">";
    $upper_tr .= "            <img src=\"/design_template/m/images/common/btn_list_detail_off.png\" alt=\"▲\">"; //#8 상세닫기
    $upper_tr .= "        </div>";
    $upper_tr .= "    </td>";
    $upper_tr .= "</tr>";

    $lower_tr .= "<tr class=\"_orderDetails\" id=\"detail%s\">";
    $lower_tr .= "</tr>";

    $sum_sell_price      = 0;
    $sum_grade_sale_price = 0;
    $sum_event_price      = 0;

    $i = 1;
    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $cate_sortcode = $fields["cate_sortcode"];

        $order_common_seqno = $fields["order_common_seqno"];
        $order_regi_date    = $fields["order_regi_date"];
        $title              = $fields["title"];
        $order_detail       = $fields["order_detail"];
        $sell_price         = doubleval($fields["sell_price"]);
        $grade_sale_price   = doubleval($fields["grade_sale_price"]);
        $member_sale_price  = doubleval($fields["member_sale_price"]);
        $add_opt_price      = doubleval($fields["add_opt_price"]);
        $add_after_price    = doubleval($fields["add_after_price"]);
        $event_price        = doubleval($fields["event_price"]);
        $expec_weight       = $fields["expec_weight"];
        $amt                = $fields["amt"];
        $amt_unit_dvs       = $fields["amt_unit_dvs"];
        $count              = $fields["count"];

        if (empty($expec_weight) === true || $expec_weight === '0') {
            $expec_weight = '-';
        } else {
            $expec_weight .= "Kg";
        }

        // 가격합산 정보 생성
        $sum_sell_price        += $sell_price;
        $sum_grade_sale_price  += $grade_sale_price * -1;
        $sum_member_sale_price += $member_sale_price * -1;
        $sum_event_price       += $event_price;
        $sum_add_after_price   += $add_after_price;
        $sum_add_opt_price     += $add_opt_price;

        $sell_price += $grade_sale_price;
        $sell_price += $member_sale_price;
        $sell_price += $add_after_price;
        $sell_price += $add_opt_price;

        // 전체 html 생성
        $ret .= sprintf($upper_tr, $order_common_seqno //#0
            , $order_common_seqno //#0
            , $order_common_seqno //#0
            , $order_common_seqno //#1
            , $title              //#1
            , number_format($sell_price) //#2
            , $i //#3
            , $i //#7
            , $i //#7
            , $order_common_seqno //#7
            , $i                  //#8
            , $i);                //#8

        $ret .= sprintf($lower_tr, $i++);

        $rs->MoveNext();
    }

    $price_info_arr["sell"]  = $sum_sell_price +
        $sum_add_after_price +
        $sum_add_opt_price;
    $price_info_arr["grade"] = $sum_grade_sale_price;
    $price_info_arr["member"] = $sum_member_sale_price;
    //$price_info_arr["event"] = $sum_event_price;
    $price_info_arr["sum"] = $sum_sell_price +
        $sum_add_after_price +
        $sum_add_opt_price -
        $sum_grade_sale_price -
        $sum_member_sale_price -
        $sum_event_price;

    return $ret;
}

?>
