<?
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . "/common_define/common_info.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");

/**
 * @brief 회원 등급 할인 dl html 반환
 *
 * @param $param = 회원등급, 할인율 파라미터
 *
 * @return dl html
 */
function getGradeSaleDl($param) {
    $util = new FrontCommonUtil();

    $mobile_yn = $param["mobile_yn"];

    $rate  = $param["rate"];

    $grade = $param["grade"];
    $grade = GRADE_EN[$grade];

    $price = number_format(doubleval($param["price"]));

    $dscr  = $param["dscr"];
    if ($dscr === null) {
        $add_dvcr = '';

        if ($param["member_sale_rate"] > 0) {
            $add_dscr = " + 추가 " . $param["member_sale_rate"] . '%';
        }

        $dscr  = sprintf("%s (할인율 : %s%%%s)", $grade, $rate, $add_dscr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <dl>
                <dt>회원등급 할인</dt>
                <dd>
                    <button type="button" class="gradeBenefit">등급할인혜택</button>
                </dd>
                <dd class="description">$dscr</dd>
                <dd id="grade_sale" class="discountAmount">$price 원</dd>
            </dl>
html;
    } else {
        $html = <<<html
                <tr>
                    <th class="order_summary_title">등급할인</th>
                    <td>$dscr</td>
                    <td style="text-align:right;">
                        <span id="grade_sale" class="order_summary_text_st">$price원</span>
                    </td>
                </tr>
html;
    }

    return $html;
}

/**
 * @brief 회원 수량별 할인 dl html 반환
 *
 * @param $param = 할인요율, 할인금액
 *
 * @return dl html
 */
function getAmtMemberSale($param) {
    $util = new FrontCommonUtil();

    $mobile_yn = $param["mobile_yn"];

    $rate = $param["rate"];
    $ap   = $param["aplc_price"];

    $price = number_format(doubleval($param["price"]));

    if (!$mobile_yn) {
        $html = <<<html
            <dl>
                <dt>회원특별 할인</dt>
                <dd>
                    <button type="button"><img src="/design_template/images/product/discount_btn_amt_member.png" alt="특별할인혜택"></button>
                </dd>
                <dd class="description">특별할인금액</dd>
                <dd id="amt_member_sale" class="discountAmount">$price 원</dd>
            </dl>
html;
    } else {
        $html = <<<html
                <tr>
                    <td class="order_summary_title">회원할인</td>
                    <td>회원할인</td>
                    <td style="text-align:right;">
                        <span id="amt_member_sale" class="order_summary_text_st">$price원</span>
                    </td>
                </tr>
html;
    }

    return $html;
}

/**
 * @brief 이벤트 할인 dl html 반환
 *
 * @param $param = 이벤트명, 할인 요율/가격 정보 파라미터
 *
 * @return dl html
 */
function getEventSaleDl($param) {
    return '';

    $mobile_yn = $param["mobile_yn"];

    $name  = $param["name"];
    $price = $param["price"];
    $dscr  = $param["dscr"];

    if (!$mobile_yn) {
        $html = <<<html
            <dl>
                <dt>이벤트 할인</dt>
                <dd>
                    <button type="button"><img src="/design_template/images/product/discount_btn_event.png" alt="관련 이벤트"></button>
                </dd>
                <dd class="description">$dscr</dd>
                <dd id="event_sale" class="discountAmount"></dd>
            </dl>
html;
    } else {
        $html = <<<html
                <tr>
                    <td class="order_summary_title">이벤트할인</td>
                    <td>이벤트할인 $dscr</td>
                    <td style="text-align:right;">
                        <span id="event_sale" class="order_summary_text_st">$price원</span>
                    </td>
                </tr>
html;
    }

    return $html;
}
?>
