<?
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");

/**
 * @brief 추가 옵션 정보 html 반환
 *
 * @param $idx      = 구분용 인덱스
 * @param $opt_name = 옵션명
 * @param $info_arr = 추가옵션 정보
 *
 * @return div html OR input hidden html
 */
function getAddOptInfoHtml($idx, $opt_name, $info_arr, $mobile_yn, $is_ao, $def) {
    $util = new FrontCommonUtil();

    $dvs = $info_arr["dvs"];
    unset($info_arr["dvs"]);

    $flag = false;
    $attr = '';

    $option = null;
    $note = '';

    if ($opt_name === "빠른생산요청") {
        $mpcode = $def["mpcode"];
        $option = option("빠른생산요청", $mpcode, "selected=\"selected\"");
    } else {
        $info_arr_count = count($info_arr);
        for ($i = 0; $i < $info_arr_count; $i++) {
            $info = $info_arr[$i];

            $mpcode = $info["mpcode"];
            $depth1 = $info["depth1"];
            $out_date = date('Y년 m월 d일', $info_arr[$i]["out_date"]);

            if($depth1 === "오늘출고") {
                $out_date_html .= "<input type='hidden' id='out_date1' value='" . $out_date . "'>";
            }
            if($depth1 === "내일출고")
                $out_date_html .= "<input type='hidden' id='out_date2' value='". $out_date ."'>";

            if ($opt_name === "포장방법") {
                $opt = $info["depth2"];
            } else {
                $opt = $util->getOptAfterFullName($info);
            }

            if ($opt === '') {
                $flag = true;
                break;
            }

            $selected = '';
            if ($mpcode === $def["mpcode"]) {
                $selected = "selected=\"selected\"";
            }

            $option .= option($opt, $mpcode, $selected);
            $flag = false;
        }

        if ($opt_name === "시안요청") {
            $note = "시안확인을 하지 않으시면 인쇄가 진행되지 않습니다.";
        } else if ($opt_name === "베다인쇄") {
            $note = "제품 납기가 연장될 수 있습니다.";
        } else if ($opt_name === "색견본참고") {
            $note = "색견본 도착 후 인쇄가 진행됩니다.";
        } else if ($opt_name === "빠른출고") {
            $note = "12시 이후 결제건은 출고일이 하루지연됩니다.<br><div id='div_out_date'></div>";
        } else if ($opt_name === "당일판") {
            $note = "<div style='color:red;'>당일판 주문 마감시간은 " . explode(" ", $info["depth1"])[0] . "입니다. 마감후 주문은 본판으로 접수/제작되어 출고 됩니다.</div>";
        }
    }

    $func_name = "loadOptPrice.exec";
    if ($is_ao) {
        $func_name = "loadAoOptPrice.exec";
    }

    if (!$mobile_yn) {
        if (!empty($note)) {
            $note  = "<dd class=\"br note\" style=\"float:left; width:83%;\">"
                     . $note . "</dd>";
        }
        $html_sel = <<<html_sel
            <div id="opt_{$idx}_div" class="option">
            <dl>
                <dt>$opt_name</dt>
                <dd class="price" id="opt_{$idx}_price"></dd>
                <dd>
                    <select id="opt_{$idx}_val" onchange="changeData();" {$attr}>
                        $option
                    </select>
                </dd>
                {$note}
                $out_date_html
            </dl>
            </div>
html_sel;
    } else {
        $html_sel = <<<html_sel
            <table class="basic_table" id="opt_{$idx}_table">
                <colgroup>
                    <col style="width:30%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>{$opt_name}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div class="after_price" id="opt_{$idx}_price"></div>
                            <div class="toggle" action="opt" idx="{$idx}">
                            </div>
                            <input type="checkbox" name="chk_opt" id="opt_{$idx}" value="{$opt_name}" price="" onclick="{$func_name}(this, '{$idx}', '{$dvs}');" style="display:none;" />
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <select class="basic_input" id="opt_{$idx}_val" onchange="changeData();" {$attr}>
                            $option
                        </select>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <span class="order_after_note">{$note}</span>
                    </td>
                </tr>
            </table>
html_sel;
    }

    // 하위 depth 가 없는 경우
    if (!$mobile_yn) {
        $html_hidden = <<<html_hidden
            <div id="opt_{$idx}_div" class="option">
            <dl>
                <dt>$opt_name</dt>
                <dd class="price" id="opt_{$idx}_price"></dd>
                {$note}
                <input type="hidden" id="opt_{$idx}_val" value="{$mpcode}" style="display:none;" />
            </dl>
            </div>
html_hidden;
    } else {
        $html_hidden = <<<html_hidden
            <table class="basic_table" id="opt_{$idx}_table">
                <colgroup>
                    <col style="width:30%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>{$opt_name}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div class="after_price" id="opt_{$idx}_price">0원</div>
                            <div class="toggle" action="opt" idx="{$idx}">
                            </div>
                            <input type="checkbox" name="chk_opt" id="opt_{$idx}" value="{$opt_name}" price="" onclick="changeData();" style="display:none;" />
                        </div>
                        <input type="hidden" id="opt_{$idx}_val" value="{$mpcode}" style="display:none;" />
                    </td>
                </tr>
            </table>
html_hidden;
    }

    if ($flag) {
        return $html_hidden;
    }

    return $html_sel;
}

function getAddAoOptInfoHtml($idx, $opt_name, $info_arr, $mobile_yn, $is_ao, $def) {
    $util = new FrontCommonUtil();

    $dvs = $info_arr["dvs"];
    unset($info_arr["dvs"]);

    $flag = false;
    $attr = '';

    $option = null;
    $note = '';

    if ($opt_name === "빠른생산요청") {
        $mpcode = $def["mpcode"];
        $option = option("빠른생산요청", $mpcode, "selected=\"selected\"");
    } else {
        $info_arr_count = count($info_arr);
        for ($i = 0; $i < $info_arr_count; $i++) {
            $info = $info_arr[$i];

            $mpcode = $info["mpcode"];

            if ($opt_name === "포장방법") {
                $opt = $info["depth2"];
            } else {
                $opt = $util->getOptAfterFullName($info);
            }

            if ($opt === '') {
                $flag = true;
                break;
            }

            $selected = '';
            if ($mpcode === $def["mpcode"]) {
                $selected = "selected=\"selected\"";
            }

            $option .= option($opt, $mpcode, $selected);
            $flag = false;
        }

        if ($opt_name === "시안요청") {
            $note = "시안확인을 하지 않으시면 인쇄가 진행되지 않습니다.";
        } else if ($opt_name === "베다인쇄") {
            $note = "제품 납기가 연장될 수 있습니다.";
        } else if ($opt_name === "색견본참고") {
            $note = "색견본 도착 후 인쇄가 진행됩니다.";
        }
    }

    $func_name = "loadOptPrice.exec";
    if ($is_ao) {
        $func_name = "loadAoOptPrice.exec";
    }

    $count_option = '';
    for ($i = 1; $i <=50; $i++) {
        $count_option .= option($i . '개', $i);
    }

    if (!$mobile_yn) {
        if (!empty($note)) {
            $note  = "<dd class=\"br note\" style=\"float:left; width:83%;\">"
                . $note . "</dd>";
        }
        $html_sel = <<<html_sel
            <div id="opt_{$idx}_div" class="option">
            <dl>
                <dt>$opt_name</dt>
                <dd class="price" id="opt_{$idx}_price"></dd>
                <dd>
                    <select id="opt_{$idx}_val" onchange="changeData();" {$attr}>
                        $option
                    </select>
                    <select id="opt_{$idx}_cnt" onchange="changeData();">
                        {$count_option}
                    </select>
                </dd>
                {$note}
            </dl>
            </div>
html_sel;
    } else {
        $html_sel = <<<html_sel
            <table class="basic_table" id="opt_{$idx}_table">
                <colgroup>
                    <col style="width:30%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>{$opt_name}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div class="after_price" id="opt_{$idx}_price"></div>
                            <div class="toggle" action="opt" idx="{$idx}">
                            </div>
                            <input type="checkbox" name="chk_opt" id="opt_{$idx}" value="{$opt_name}" price="" onclick="{$func_name}(this, '{$idx}', '{$dvs}');" style="display:none;" />
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <select class="basic_input" id="opt_{$idx}_val" onchange="changeData();" {$attr}>
                            $option
                        </select>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <span class="order_after_note">{$note}</span>
                    </td>
                </tr>
            </table>
html_sel;
    }

    // 하위 depth 가 없는 경우
    if (!$mobile_yn) {
        $html_hidden = <<<html_hidden
            <div id="opt_{$idx}_div" class="option">
            <dl>
                <dt>$opt_name</dt>
                <dd class="price" id="opt_{$idx}_price"></dd>
                {$note}
                <input type="hidden" id="opt_{$idx}_val" value="{$mpcode}" style="display:none;" />
            </dl>
            </div>
html_hidden;
    } else {
        $html_hidden = <<<html_hidden
            <table class="basic_table" id="opt_{$idx}_table">
                <colgroup>
                    <col style="width:30%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>{$opt_name}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div class="after_price" id="opt_{$idx}_price">0원</div>
                            <div class="toggle" action="opt" idx="{$idx}">
                            </div>
                            <input type="checkbox" name="chk_opt" id="opt_{$idx}" value="{$opt_name}" price="" onclick="{$func_name}(this, '{$idx}', '{$dvs}');" style="display:none;" />
                        </div>
                        <input type="hidden" id="opt_{$idx}_val" value="{$mpcode}" style="display:none;" />
                    </td>
                </tr>
            </table>
html_hidden;
    }

    if ($flag) {
        return $html_hidden;
    }

    return $html_sel;
}


/**
 * @brief 추가 옵션 추가물품 정보 html 반환
 *
 * @param $idx  = 구분용 인덱스
 * @param $info = 옵션 정보
 *
 * @return div html
 */
function getAddGoodsInfoHtml($idx, $opt_name, $info, $mobile_yn) {
    $dvs = $info["dvs"];
    unset($info["dvs"]);

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $depth3 = $temp["depth3"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2][$depth3] = $mpcode;
    }

    $depth1_option = "";
    $depth2_option = "";
    $depth3_option = "";

    $line_manu = false;

    $d2_flag = true;
    $d3_flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $attr = "";

        if ($d2_flag === true) {
            $d2_flag = false;

            foreach ($depth2_arr as $depth2 => $depth3_arr) {
                $depth2_option .= option($depth2, $depth2);

                if ($d3_flag === true) {
                    $d3_flag = false;

                    foreach ($depth3_arr as $depth3 => $mpcode) {
                        $depth3_option .= option($depth3, $mpcode);
                    }
                }
            }

            $attr = "selected=\"selected\"";
        }

        $depth1_option .= option($depth1, $depth1, $attr);
    }

    $amt_option = '';
    for ($i = 1; $i <= 40; $i++) {
        $amt_option .= option($i . '개', $i . '개');
    }

    $attr = "style=\"min-width:0px;\"";

    if (!$mobile_yn) {
        $html = <<<html
            <div id="opt_{$idx}_div" class="option">
                <dl>
                    <dt>{$opt_name}</dt>
                    <dd class="price" id="opt_{$idx}_price"></dd>
                    <dd>
                        <select id="opt_{$idx}_depth1" onchange="loadAddGoodsDepth('{$idx}', this.value, '{$dvs}', '1');" {$attr}>
                            $depth1_option
                        </select>
                        &nbsp;굵기
                        <select id="opt_{$idx}_depth2" onchange="loadAddGoodsDepth('{$idx}', this.value, '{$dvs}', '2');" {$attr}>
                            $depth2_option
                        </select>
                        &nbsp;길이
                        <select id="opt_{$idx}_val" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                            $depth3_option
                        </select>
                        &nbsp;수량
                        <select id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                            $amt_option
                        </select>
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
            <table class="basic_table">
                <colgroup>
                    <col style="width:20%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>{$opt_name}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div id="opt_{$idx}_price" class="after_price"></div>
                            <div class="toggle" action="opt" idx="{$idx}"></div>
                            <input type="checkbox" name="chk_opt" id="opt_{$idx}" value="{$opt_name}" price="" onclick="loadAoOptPrice.exec(this, '{$idx}', '{$dvs}');" style="display:none;" />
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:20%;">
                                <col style="width:80%;">
                            </colgroup>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">품목</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_depth1" onchange="loadAddGoodsDepth('{$idx}', this.value, '{$dvs}', '1');" {$attr}>
                                        $depth1_option
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">굵기</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_depth2" onchange="loadAddGoodsDepth('{$idx}', this.value, '{$dvs}', '2');" {$attr}>
                                        $depth2_option
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">길이</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_val" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                                        $depth3_option
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">수량</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                                        $amt_option
                                    </select>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 옵션 로프 정보 html 반환
 *
 * @param $idx  = 구분용 인덱스
 * @param $info = 옵션 정보
 *
 * @return div html
function getRopeInfoHtml($idx, $info, $dvs, $mobile_yn) {
    $dvs = $info["dvs"];
    unset($info["dvs"]);

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2] = $mpcode;
    }

    $depth1_option = "";
    $depth2_option = "";

    $line_manu = false;

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $attr = "";

        if ($flag === true) {
            $flag = false;

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $depth2_option .= option($depth2, $mpcode);
            }

            $attr = "selected=\"selected\"";
        }

        $depth1_option .= option($depth1, $depth1, $attr);
    }

    $amt_option = '';
    for ($i = 1; $i <= 40; $i++) {
        $amt_option .= option($i . '개', $i . '개');
    }

    $attr = "style=\"min-width:0px;\"";

    if (!$mobile_yn) {
        $html = <<<html
            <div id="opt_{$idx}_div" class="option">
                <dl>
                    <dt>로프</dt>
                    <dd class="price" id="opt_{$idx}_price"></dd>
                    <dd>
                        굵기
                        <select id="opt_{$idx}_depth1" onchange="loadRopeDepth2('{$idx}', this.value, '{$dvs}');" {$attr}>
                            $depth1_option
                        </select>
                        &nbsp;길이
                        <select id="opt_{$idx}_val" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                            $depth2_option
                        </select>
                        &nbsp;수량
                        <select id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                            $amt_option
                        </select>
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
            <table class="basic_table">
                <colgroup>
                    <col style="width:20%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>로프</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div id="opt_{$idx}_price" class="after_price"></div>
                            <div class="toggle"></div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:20%;">
                                <col style="width:80%;">
                            </colgroup>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">굵기</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_depth1" onchange="loadRopeDepth2('{$idx}', this.value, '{$dvs}');" {$attr}>
                                        $depth1_option
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">길이</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_val" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                                        $depth2_option
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">수량</span>
                                </th>
                                <td style="padding:10px 0 !important;">
                                    <select class="basic_input Column_01" id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                                        $amt_option
                                    </select>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
html;
    }

    return $html;
}
 */

/**
 * @brief 추가 옵션 로프 정보 html 반환
 *
 * @param $idx  = 구분용 인덱스
 * @param $info = 옵션 정보
 *
 * @return div html
function getKyubangInfoHtml($idx, $info, $mobile_yn) {
    $mpcode = $info[0]["mpcode"];

    $amt_option = '';
    for ($i = 1; $i <= 40; $i++) {
        $amt_option .= option($i . '개', $i . '개');
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div id="opt_{$idx}_div" class="option">
                <dl>
                    <dt>큐방</dt>
                    <dd class="price" id="opt_{$idx}_price"></dd>
                    <dd>
                        <select id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                            $amt_option
                        </select>
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
            <table class="basic_table">
                <colgroup>
                    <col style="width:20%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>로프</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div id="opt_{$idx}_price" class="after_price"></div>
                            <div class="toggle"></div>
                            <input type="hidden" id="opt_{$idx}_val" value="{$mpcode}" style="display:none;" />
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:20%;">
                                <col style="width:80%;">
                            </colgroup>
                            <tr>
                                <th style="font-size:1em;">
                                    <span class="order_size_form_text_after">수량 </span>
                                </th>
                                <td>
                                    <select class="basic_input Column_01" id="opt_{$idx}_info" onchange="loadAoOptPrice.calc('{$idx}', this.value);" {$attr}>
                                        $amt_option
                                    </select>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
html;
    }

    return $html;
}
 */
?>

