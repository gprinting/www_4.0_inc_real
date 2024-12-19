<?
function orderStatus($param) {
    $state_arr = $param["state_arr"];

    $tot_cnt = intval($param["tot_cnt"]);
    $tot_cnt = $tot_cnt < 0 ?  0 : $tot_cnt;

    $html = <<<HTML

        <dl class="orderNum all" onclick="goPage('/mypage22/order_all.html');" style="cursor:pointer;">
            <dt>전체주문</dt>
            <dd id="tot_cnt">$tot_cnt</dd>
        </dl>
        <dl class="orderNum cart" onclick="goPage('/mypage22/cart.html');" style="cursor:pointer;">
            <dt>장바구니</dt>
            <dd>$state_arr[장바구니]</dd>
        </dl>
        <ul class="byStatus _toggle">
            <li class="order _on" id="order">
                <button type="button" onclick="goPage('/mypage22/cart.html');">
                    <span class="name">장바구니</span>
                    <span class="total" id="">$state_arr[장바구니]</span>
                </button>
            </li>
            <li class="waiting" id="waiting">
                <button type="button" onclick="goOrderAll('입금');">
                    <span class="name">결제</span>
                    <span class="total" id="waiting_cnt">$state_arr[입금]</span>
                </button>
            </li>
            <li class="application" id="application">
                <button type="button" onclick="goOrderAll('접수');">
                    <span class="name">접수</span>
                    <span class="total" id="application_tot_cnt">$state_arr[접수]</span>
                </button>
            </li>
            <li class="set" id="set">
                <button type="button" onclick="goOrderAll('조판');">
                    <span class="name">조판</span>
                    <span class="total">$state_arr[조판]</span>
                </button>
            </li>
            <li class="print" id="print">
                <button type="button" onclick="goOrderAll('출력');">
                    <span class="name">출력</span>
                    <span class="total">$state_arr[출력]</span>
                </button>
            </li>
            <li class="process" id="process">
                <button type="button" onclick="goOrderAll('인쇄');">
                    <span class="name">인쇄</span>
                    <span class="total">$state_arr[인쇄]</span>
                </button>
            </li>
            <li class="post" id="post">
                <button type="button" onclick="goOrderAll('후가공');">
                    <span class="name">후가공</span>
                    <span class="total">$state_arr[후공정]</span>
                </button>
            </li>
            <li class="stock" id="stock">
                <button type="button" onclick="goOrderAll('입고');">
                    <span class="name">입고</span>
                    <span class="total">$state_arr[입고]</span>
                </button>
            </li>
            <li class="release" id="release">
                <button type="button" onclick="goOrderAll('출고');">
                    <span class="name">출고</span>
                    <span class="total">$state_arr[출고]</span>
                </button>
            </li>
            <li class="delivery" id="delivery">
                <button type="button" onclick="goOrderAll('배송');">
                    <span class="name">배송</span>
                    <span class="total">$state_arr[배송]</span>
                </button>
            </li>
            <li class="complete" id="complete">
                <button type="button" onclick="goOrderAll('완료');">
                    <span class="name">완료</span>
                    <span class="total">$state_arr[구매확정]</span>
                </button>
            </li>
            <li class="cancel" id="cancel">
                <button type="button">
                    <span class="name">취소</span>
                    <span class="total">0</span>
                </button>
            </li>
        </ul>
HTML;

        //<table class="list ordered _details" id="order_list">
        //    $param[list]
        //</table>

    return $html;
}
?>
