<?
/**
 * @brief 상품 선택 팝업 html 생성
 *
 * @param $rs = 검색결과
 *
 * @return 팝업 html
 */
function productListPopup($rs, $to, $selected) {
    $tr_base  = "<tr>";
    $tr_base .= "<td><label class=\"check\"><input id=\"available_%s\" type=\"checkbox\" name=\"product_ck\" value=\"%s\" class=\"popupProducts\" %s><i><i></i></i></label></td>";
    $tr_base .= "<td>%s</td>";
    $tr_base .= "<td>%s</td>";
    $tr_base .= "<td>%s%s</td>";
    $tr_base .= "</tr>";

    $tr = "";
    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $amt   = '';
        $count = '';

        if (empty($fields["s_amt"]) === false) {
            $amt   = $fields["s_amt"];
            $count = '('. $fields["s_count"] . ')';
        }

        if (empty($fields["b_amt"]) === false) {
            if (empty($amt) === true) {
                $amt   = $fields["b_amt"];
                $count = '(1)';
            } else {
                $amt   = "혼합형";
            }
        }

        $tr .= sprintf($tr_base
            , $fields["order_common_seqno"]
            , $fields["order_common_seqno"]
            , ""
            , $fields["title"]
            , $fields["order_detail"]
            , $amt
            , $count);

        $rs->MoveNext();
    }

    $table = "<strong><button class=\"btn_type_02\" onclick=\"setProducts('%s');\">적용</button></strong>";
    $table1 = sprintf($table, $to);

    $html = <<<html
        <button class="btn_popup_close close" title="닫기"><img src="/design_template/images/common/btn_circle_x_white.png" alt="X"></button>
        <div class="inner_padding">
            <div class="layerPopup">
                <div>
                    <h2>선택한 주문 목록</h2>
                </div>
                <div class="layerPopup_contents">
                    <div>
                        <table class="list thead">
                            <colgroup>
                                <col width="50">
                                <col width="120">
                                <col width="">
                                <col width="80">
                            </colgroup>
                            <thead>
                                <tr>
                                    <th><label class="check"><input type="checkbox" id="cb_chooseorder" onclick="changestate()"><i><i></i></i></label></th>
                                    <th>인쇄물제목</th>
                                    <th>상품정보</th>
                                    <th>수량(건)</th>
                                </tr>
                            </thead>
                        </table>
                        <div class="tableScroll">
                            <div class="wrap">
                                <table class="list">
                                    <colgroup>
                                        <col width="50">
                                        <col width="120">
                                        <col width="">
                                        <col width="70">
                                    </colgroup>
                                    <tbody>
                                        $tr
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="btn_wrapper">
                        $table1
                    <button type="button" class="btn_type_01 close">취소</button>
                </div>
            </div>
        </div>
        
html;

    return $html;
}



/**
 * @brief 나의배송지 선택 팝업 html 생성
 *
 * @param $rs = 검색결과
 *
 * @return 팝업 html
 */
function addressListPopup($rs) {
    $tr_base  = "<tr>";
    $tr_base .= "    <td>%s</td>"; // 별칭
    $tr_base .= "    <td>%s</td>"; // 받으시는분
    $tr_base .= "    <td class=\"address\">%s %s</td>"; // 주소
    $tr_base .= "    <td class=\"btn\">";
    $tr_base .= "       <button type=\"button\" class=\"location\" name=\"cb_order\" onclick=\"setMemberAddrInfo.exec(this);\">선택</button>";
    $tr_base .= "       <input type=\"hidden\" name=\"name\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"recei\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"tel_num\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"cell_num\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"zipcode\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"addr\" value=\"%s\" />";
    $tr_base .= "       <input type=\"hidden\" name=\"addr_detail\" value=\"%s\" />";
    $tr_base .= "    </td>";
    $tr_base .= "</tr>";

    $tr = "";

    if ($rs->EOF) {
        $tr = "<tr><td colspan=\"4\" class=\"noResult\">나의배송지 정보가 없습니다.</td></tr>";
    }

    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $tr .= sprintf($tr_base, $fields["dlvr_name"]
                               , $fields["recei"]
                               , $fields["addr"]
                               , $fields["addr_detail"]
                               , $fields["dlvr_name"]
                               , $fields["recei"]
                               , $fields["tel_num"]
                               , $fields["cell_num"]
                               , $fields["zipcode"]
                               , $fields["addr"]
                               , $fields["addr_detail"]);
        $rs->MoveNext();
    }

    $html = <<<html
        <header class="popup">
            <h2>나의 배송지 목록</h2>
            <button type="button" class="close">닫기</button>
        </header>
        <article>
            <section class="searchCondition">
                <div class="rowWrap">
                    <dl class="delivery full">
                        <dt>검색조건</dt>
                        <dd class="rest">
                            <input type="text" id="searchkey" name="searchkey" onkeyup="if(window.event.keyCode==13){searchDlvrList()}">
                        </dd>
                    </dl>
                </div>
                <button onclick="searchDlvrList();" class="search">검색</button>
            </section>
            <table class="list info thead">
                <colgroup>
                    <col class="tm">
                    <col class="name">
                    <col class="address">
                    <col class="btn">
                </colgroup>
                <thead>
                    <tr>
                        <th>업체/상호명</th>
                        <th>담당자</th>
                        <th>주소</th>
                        <th></th>
                    </tr>
                </thead>
            </table>
            <div class="tableScroll">
                <div class="wrap">
                    <table class="list info">
                        <colgroup>
                            <col class="tm">
                            <col class="name">
                            <col class="address">
                            <col class="btn">
                        </colgroup>
                        <tbody id="address_list">
                            $tr
                        </tbody>
                    </table>
                </div>
            </div>
        </article>

html;
    
    return $html;
}

/**
 * @brief 포인트 사용 팝업 html 생성
 *
 * @param $own_point = 보유포인트
 *
 * @return 팝업 html
 */
function pointPopup($own_point, $pay_price) {
    $pay_price = str_replace(",", "", $pay_price);
    $own_point = str_replace(",", "", $own_point);

    if ($own_point > $pay_price) {
        $max_use_point = number_format(doubleval($pay_price));
    } else {
        $max_use_point = number_format(doubleval($own_point));
    }
    $own_point = number_format(doubleval($own_point));

    $html = <<<html
        <header class="popup">
            <h2>포인트 현황</h2>
            <button type="button" class="close">닫기</button>
        </header>
        <article>
            <table class="sheetForm horizontal">
                <colgroup>
                    <col class="th">
                    <col>
                </colgroup>
                <tbody>
                    <tr>
                        <th scope="row">보유 포인트</td>
                        <td><label><input type="text" id="own_point" readonly value="$own_point"> P</label></td>
                    </tr>
                    <tr>
                        <th scope="row">최대 사용 가능 포인트</td>
                        <td><label><input type="text" id="max_use_point" readonly value="$max_use_point"> P</label></td>
                    </tr>
                    <tr>
                        <th scope="row">사용 포인트</td>
                        <td><input type="text" id="use_point" value="0"> P</label></td>
                    </tr>
                </tbody>
            </table>
            <p class="note box center">포인트는 백원 단위로 사용이 가능합니다.</p>
            <hr class="end">
            <footer class="action">
                <button class="progress" onclick="setPointPrice();">사용</button>
                <button class="close">취소</button>
            </footer>
        </article>
html;

    return $html;
}

/**
 * @brief 쿠폰 사용 팝업 html 생성
 *
 * @param $tbody = 쿠폰목록
 *
 * @return 팝업 html
 */
function couponPopup($tbody) {
    $html = <<<html
        <header class="popup">
            <h2>쿠폰 현황</h2>
            <button type="button" class="close">닫기</button>
        </header>
        <article>
            <table class="list info thead">
                <colgroup>
                    <col class="check">
                    <col class="title">
                    <col class="amount">
                    <col class="period">
                </colgroup>
                <thead>
                    <tr>
                        <th><label class="check"><input type="checkbox" class="_general"><i><i></i></i></label></th>
                        <th>쿠폰명</th>
                        <th>할인금액</th>
                        <th>기간</th>
                    </tr>
                </thead>
            </table>
            <div class="tableScroll">
                <div class="wrap">
                    <table class="list info">
                        <colgroup>
                            <col class="check">
                            <col class="title">
                            <col class="amount">
                            <col class="period">
                        </colgroup>
                        <tbody>
                            $tbody
                        </tbody>
                    </table>
                </div>
            </div>
            <hr class="end">
            <footer class="action">
                <button type="button" class="progress" onclick="applyCoupon();">적용</button></strong>
                <button type="button" class="close">취소</button>
            </footer>
        </article>
        <script>generalCheckbox();</script>
html;

    return $html;
}
?>
