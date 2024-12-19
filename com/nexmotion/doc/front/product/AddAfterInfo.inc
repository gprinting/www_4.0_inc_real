<?
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");

/**
 * @brief 추가 후공정 코팅 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getCoatingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $util = new FrontCommonUtil();

    $aft_en = "coating";
    $aft_ko = "코팅";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $option = "";
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];

        $mpcode = $temp["mpcode"];
        $aft = $temp["depth1"] . ' ' . $temp["depth2"];

        $class = "";

        if (strpos($aft, "부분") !== false) {
            $class = "class=\"_part\"";
        }

        $selected = "";
        if ($def["mpcode"] === $mpcode) {
            $selected = "selected=\"selected\"";
        }

        $option .= option($aft, $mpcode, $selected . ' ' .  $class);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $option
                        </select>
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                    <dd class="br note">
                            질감이 있는종이 코팅시 들뜸현상이 생길수있습니다.
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                    <table class="basic_table after_{$aft_en}">
                        <colgroup>
                            <col style="width:20%;">
                            <col style="">
                        </colgroup>
                        <tr>
                            <th>{$aft_ko}</th>
                            <td>
                                <div></div>
                                <div class="toggle_wrapper">
                                    <div id="{$prefix}_price_dd" class="after_price"></div>
                                    <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                    <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');" class="basic_input">
                                    $option
                                </select>
                                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            </td>
                        </tr>
                    </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 귀도리 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getRoundingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "rounding";
    $aft_ko = "귀도리";
    $prefix = $dvs . '_' . $aft_en;

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

    $pass = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $attr = "";

        if ($pass && ($depth1 === $def["depth1"] || $depth1 === "네귀도리")) {
            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_selected = "";
                if ($def["depth2"] === $depth2 || $depth2 === "4mm") {
                    $d2_selected = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_selected);
            }

            $attr = "selected=\"selected\"";

            $pass = false;
        }

        if ($depth1 === "네귀도리") {
            $attr .= " class=\"_all\"";
        }

        $depth1_option .= option($depth1, $depth1, $attr);
    }

    // 체크박스 옵션값 선택(자주이용하는 상품에서 넘어온 경우)
    $lt = '';
    $rt = '';
    $lb = '';
    $rb = '';
    $chk_info = explode('|', $def["info"]);
    foreach ($chk_info as $chk) {
        switch ($chk) {
            case "좌상" :
                $lt = "checked=\"checked\"";
                break;
            case "우상" :
                $rt = "checked=\"checked\"";
                break;
            case "우하" :
                $lb = "checked=\"checked\"";
                break;
            case "좌하" :
                $rb = "checked=\"checked\"";
                break;
        }
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select class="_num" id="{$prefix}_cnt" onchange="loadRoundingDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                    </dd>
                    <dd class="br">
                        <label class="left top"><input name="{$prefix}_dvs" class="aft_chkbox" value="좌상" type="checkbox" onchange="chkRoundingLimit('{$dvs}', this);" {$lt}> 좌상</label>
                        <label class="right top"><input name="{$prefix}_dvs" class="aft_chkbox" value="우상" type="checkbox" onchange="chkRoundingLimit('{$dvs}', this);" {$rt}> 우상</label>
                        <label class="right bottom"><input name="{$prefix}_dvs" class="aft_chkbox" value="우하" type="checkbox" onchange="chkRoundingLimit('{$dvs}', this);" {$lb}> 우하</label>
                        <label class="left bottom"><input name="{$prefix}_dvs" class="aft_chkbox" value="좌하" type="checkbox" onchange="chkRoundingLimit('{$dvs}', this);" {$rb}> 좌하</label>
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02 _num" id="{$prefix}_cnt" onchange="loadRoundingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" style="padding: 0 !important;">
                            <table style="width:100%;">
                                <colgroup>
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                </colgroup>
                                <tr>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_1" class="_chk_round" name="{$prefix}_dvs" value="좌상" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_1">좌상</label>
                                        </span>
                                    </td>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_2" class="_chk_round" name="{$prefix}_dvs" value="우상" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_2">우상</label>
                                        </span>
                                    </td>
                                    <td>
                                       <span class="after_option_span">
                                           <input type="checkbox" id="{$prefix}_3" class="_chk_round" name="{$prefix}_dvs" value="우하" onchange="chkRoundingLimit('{$dvs}', this);"><label for="c{$prefix}3">우하</label>
                                        </span>
                                    </td>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_4" class="_chk_round" name="{$prefix}_dvs" value="좌하" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_4">좌하</label>
                                        </span>
                                    </td>
                                </tr>
                            </table>

                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 귀도리 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getEdgeFoilInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "edgefoil";
    $aft_ko = "엣지박";
    $prefix = $dvs . '_' . $aft_en;

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

    $pass = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $attr = "";
        $depth1_option .= option($depth1, $depth2_arr["-"], $attr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select class="_num" id="{$prefix}_val" onchange="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_cnt" name="{$prefix}_cnt" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');" style="display: none;">
                            $depth2_option
                        </select>
                    </dd>
                </dl>
                <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02 _num" id="{$prefix}_cnt" onchange="loadRoundingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" style="padding: 0 !important;">
                            <table style="width:100%;">
                                <colgroup>
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                </colgroup>
                                <tr>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_1" class="_chk_round" name="{$prefix}_dvs" value="좌상" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_1">좌상</label>
                                        </span>
                                    </td>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_2" class="_chk_round" name="{$prefix}_dvs" value="우상" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_2">우상</label>
                                        </span>
                                    </td>
                                    <td>
                                       <span class="after_option_span">
                                           <input type="checkbox" id="{$prefix}_3" class="_chk_round" name="{$prefix}_dvs" value="우하" onchange="chkRoundingLimit('{$dvs}', this);"><label for="c{$prefix}3">우하</label>
                                        </span>
                                    </td>
                                    <td>
                                        <span class="after_option_span">
                                            <input type="checkbox" id="{$prefix}_4" class="_chk_round" name="{$prefix}_dvs" value="좌하" onchange="chkRoundingLimit('{$dvs}', this);"><label for="{$prefix}_4">좌하</label>
                                        </span>
                                    </td>
                                </tr>
                            </table>

                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }
    return $html;
}


/**
 * @brief 추가 후공정 오시 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getImpressionInfoHtml($info, $dvs, $mobile_yn, $def, $aft_ko) {
    switch ($aft_ko) {
        case "오시" :
            $aft_en = "impression";
            break;
        case "도무송오시" :
            $aft_en = "thomson_impression";
            break;
    }

    $prefix = $dvs . '_' . $aft_en;

    // depth1 기본값 처리
    $one_line   = '';
    $two_line   = '';
    $three_line = '';
    $four_line  = '';
    $five_line  = '';
    $six_line  = '';
    switch ($def["depth1"]) {
        case "1줄" :
            $one_line = "selected=\"selected\"";
            break;
        case "2줄" :
            $two_line = "selected=\"selected\"";
            break;
        case "3줄" :
            $three_line = "selected=\"selected\"";
            break;
        case "4줄" :
            $four_line = "selected=\"selected\"";
            break;
        case "5줄" :
            $five_line = "selected=\"selected\"";
            break;
        case "6줄" :
            $six_line = "selected=\"selected\"";
            break;
    }
    // depth2 기본값 처리
    $one_half = '';
    $one_n_half = '';

    $two_triple = '';
    $two_cross = '';
    $two_multi = '';

    $three_fourth = '';
    $three_multi = '';

    $four_fifth = '';
    $four_multi = '';

    $fix_sixth = '';
    $fix_multi = '';

    $six_seventh = '';
    $six_multi = '';
    if (!empty($one_line)) {
        switch ($def["depth2"]) {
            case "중앙2단" :
                $one_half = "checked=\"checked\"";
                break;
            case "비중앙2단" :
                $two_n_half = "checked=\"checked\"";
                break;
        }
    } else if (!empty($two_line)) {
        switch ($def["depth2"]) {
            case "비례3단" :
                $two_triple = "checked=\"checked\"";
                break;
            case "십자4단" :
                $two_cross = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $two_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($three_line)) {
        switch ($def["depth2"]) {
            case "비례4단" :
                $three_fourth = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $three_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($four_line)) {
        switch ($def["depth2"]) {
            case "비례5단" :
                $four_fifth = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $four_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($five_line)) {
        switch ($def["depth2"]) {
            case "비례6단" :
                $five_sixth = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $five_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($six_line)) {
        switch ($def["depth2"]) {
            case "비례7단" :
                $six_seventh = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $six_multi = "checked=\"checked\"";
                break;
        }
    }
    // 위치값 기본값 처리
    $position_arr = [];
    if (!empty($def["info"])) {
        $position_arr = explode('|', $def["info"]);
    }

    $info_count = count($info);

    // html 생성하기 쉽도록 배열 가공
    $html_info = [];

    $option = "";
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];

        $mpcode = $temp["mpcode"];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];

        $html_info[$depth1][$depth2] = $mpcode;
    }

    $is_one   = false;
    $is_two   = false;
    $is_three = false;
    $is_four  = false;
    $is_five  = false;
    $is_six  = false;

    // 1줄
    $one_mpcode1 = $html_info["1줄"]["중앙"];
    $one_mpcode2 = $html_info["1줄"]["비중앙"];
    if (!empty($one_mpcode1) || !empty($one_mpcode_2)) {
        $is_one   = true;
    }
    // 2줄
    $two_mpcode1 = $html_info["2줄"]["비례3단"];
    $two_mpcode2 = $html_info["2줄"]["십자4단"];
    $two_mpcode3 = $html_info["2줄"]["비대칭다단"];
    if (!empty($two_mpcode1) ||
            !empty($two_mpcode_2) ||
            !empty($two_mpcode_3)) {
        $is_two   = true;
    }
    // 3줄
    $three_mpcode1 = $html_info["3줄"]["비례4단"];
    $three_mpcode2 = $html_info["3줄"]["비대칭다단"];
    if (!empty($three_mpcode1) || !empty($three_mpcode_2)) {
        $is_three = true;
    }
    // 4줄
    $four_mpcode1 = $html_info["4줄"]["비례5단"];
    $four_mpcode2 = $html_info["4줄"]["비대칭다단"];
    if (!empty($four_mpcode1) || !empty($four_mpcode_2)) {
        $is_four  = true;
    }

    // 5줄
    $five_mpcode1 = $html_info["5줄"]["비례6단"];
    $five_mpcode2 = $html_info["5줄"]["비대칭다단"];
    if (!empty($five_mpcode1) || !empty($five_mpcode_2)) {
        $is_five  = true;
    }

    // 6줄
    $six_mpcode1 = $html_info["6줄"]["비례7단"];
    $six_mpcode2 = $html_info["6줄"]["비대칭다단"];
    if (!empty($six_mpcode1) || !empty($six_mpcode_2)) {
        $is_six  = true;
    }
    // 오시류 단독
    $solo_mpcode = $html_info['-']['-'];

    $option = '';
    if ($is_one) {
        $cnt_option .= "<option value=\"one\" class=\"_one\" {$one_line}>1줄</option>";
    }
    if ($is_two) {
        $cnt_option .= "<option value=\"two\" class=\"_two\" {$two_line}>2줄</option>";
    }
    if ($is_three) {
        $cnt_option .= "<option value=\"three\" class=\"_three\" {$three_line}>3줄</option>";
    }
    if ($is_four) {
        $cnt_option .= "<option value=\"four\" class=\"_four\" {$four_line}>4줄</option>";
    }
    if ($is_five) {
        $cnt_option .= "<option value=\"five\" class=\"_five\" {$five_line}>5줄</option>";
    }
    if ($is_six) {
        $cnt_option .= "<option value=\"six\" class=\"_six\" {$six_line}>6줄</option>";
    }
    if (!empty($solo_mpcode)) {
        $cnt_option .= "<option value=\"-\" class=\"_thomson\">{$aft_ko}</option>";
    }

    if (!$mobile_yn) {
        // 조건때문에 doc에서 str로 변경함
        $html  = "    <div class=\"option _{$aft_en}\">";
        $html .= "        <dl>";
        $html .= "            <dt>{$aft_ko}</dt>";
        $html .= "            <dd class=\"price\" id=\"{$prefix}_price_dd\"></dd>";
        $html .= "            <dd>";
        $html .= "                <select id=\"{$prefix}_cnt\" onchange=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" class=\"{$aft_en}_cnt\">";

        $html .=                      $cnt_option;
        $html .= "                </select>";
        //<dd class="br note">
        //                            50매철 및 별도매철 원하시는경우 분리없이 보내드리며,
        //                            <br>직접 떼어내어 사용하셔야합니다.
        //                    </dd>
        //
        if (empty($solo_mpcode)) {
            $html .= "                <select id=\"{$prefix}_vh\" onchange=\"preview.{$aft_en}('{$dvs}');\">";
            $html .= "                    <option value=\"V\">세로</option>";
            $html .= "                    <option value=\"H\">가로</option>";
            $html .= "                </select>";
        }
        // 1줄
        $html .= "            <dd class=\"br _one\">";
        if (isset($one_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode1}\" dvs=\"M\" {$one_half}> 중앙2단</label>";
        }
        if (isset($one_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode2}\" class=\"_custom\" dvs=\"C\" {$one_n_half}> 비중앙2단</label>";
        }
        $html .= "            </dd>";
        if (isset($one_mpcode2)) {
            $html .= "            <dd class=\"br _one _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_one_pos1\" type=\"text\" class=\"mm {$prefix}_one_mm\" value=\"{$position_arr[0]}\">mm</label> "; 
            $html .= "            </dd>";
            $html .= "            <dd>";
            $html .= " <p class=\"note\">오시 넣을시 터질수가 있습니다.</p>";
            $html .= "            </dd>";
        }
        // 2줄
        $html .= "            <dd class=\"br _two\">";
        if (isset($two_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode1}\" dvs=\"M\" {$two_triple}> 비례3단</label>";
        }
        if (isset($two_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode2}\" dvs=\"M\" {$two_cross}> 십자4단</label>";
        }
        if (isset($two_mpcode3)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode3}\" dvs=\"C\" class=\"_custom\" {$two_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($two_mpcode3)) {
            $html .= "            <dd class=\"br _two _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_two_pos1\" type=\"text\" class=\"mm {$prefix}_two_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_two_pos2\" type=\"text\" class=\"mm {$prefix}_two_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label>";
            $html .= "            </dd>";
        }
        // 3줄
        $html .= "            <dd class=\"br _three\">";
        if (isset($three_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$three_mpcode1}\" dvs=\"M\" {$three_fourth}> 비례4단</label>";
        }
        if (isset($three_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$three_mpcode2}\" dvs=\"C\" class=\"_custom\" {$three_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($three_mpcode2)) {
            $html .= "            <dd class=\"br _three _custom\" >";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_three_pos1\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_three_pos2\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_three_pos3\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label>";
            $html .= "            </dd>";
        }

        // 4줄
        $html .= "            <dd class=\"br _four\">";
        if (isset($four_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$four_mpcode1}\" dvs=\"M\" {$four_fifth}> 비례5단</label>";
        }
        if (isset($four_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$four_mpcode2}\" dvs=\"C\" class=\"_custom\" {$four_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($four_mpcode2)) {
            $html .= "            <dd class=\"br _four _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_four_pos1\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_four_pos2\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_four_pos3\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label><br />";
            $html .= "                <label>네 번째 선 <input id=\"{$prefix}_four_pos4\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[3]}\">mm</label>";
            $html .= "            </dd>";
        }

        // 5줄
        $html .= "            <dd class=\"br _five\">";
        if (isset($five_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_five_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$five_mpcode1}\" dvs=\"M\" {$five_sixth}> 비례6단</label>";
        }
        if (isset($five_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_five_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$five_mpcode2}\" dvs=\"C\" class=\"_custom\" {$five_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($five_mpcode2)) {
            $html .= "            <dd class=\"br _five _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_five_pos1\" type=\"text\" class=\"mm {$prefix}_five_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_five_pos2\" type=\"text\" class=\"mm {$prefix}_five_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_five_pos3\" type=\"text\" class=\"mm {$prefix}_five_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label><br />";
            $html .= "                <label>네 번째 선 <input id=\"{$prefix}_five_pos4\" type=\"text\" class=\"mm {$prefix}_five_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[3]}\">mm</label><br />";
            $html .= "                <label>다섯 번째 선 <input id=\"{$prefix}_five_pos5\" type=\"text\" class=\"mm {$prefix}_five_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[3]}\">mm</label>";
            $html .= "            </dd>";
        }

        // 6줄
        $html .= "            <dd class=\"br _six\">";
        if (isset($six_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_six_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$six_mpcode1}\" dvs=\"M\" {$six_seventh}> 비례7단</label>";
        }
        if (isset($six_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_six_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$six_mpcode2}\" dvs=\"C\" class=\"_custom\" {$six_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($six_mpcode2)) {
            $html .= "            <dd class=\"br _six _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_six_pos1\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_six_pos2\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_six_pos3\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label><br />";
            $html .= "                <label>네 번째 선 <input id=\"{$prefix}_six_pos4\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[3]}\">mm</label><br />";
            $html .= "                <label>다섯 번째 선 <input id=\"{$prefix}_six_pos5\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[4]}\">mm</label><br />";
            $html .= "                <label>여섯 번째 선 <input id=\"{$prefix}_six_pos6\" type=\"text\" class=\"mm {$prefix}_six_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[5]}\">mm</label>";
            $html .= "            </dd>";
        }
        // 도무송오시 단독
        if (!empty($solo_mpcode)) {
            $html .= "            <dd style=\"display:none;\">";
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_-_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$solo_mpcode}\" dvs=\"C\" /></label>";
            $html .= "            </dd>";
        }
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_price\" name=\"{$prefix}_price\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_info\" name=\"{$prefix}_info\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_data\" name=\"{$prefix}_data\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_val\" name=\"{$prefix}_val\" value=\"\" />";
        $html .= "        </dl>";
        $html .= "    </div>";
    ///////////////////////////////////////////////////////
    } else {
    ///////////////////////////////////////////////////////
        $html .= "<table class=\"basic_table after_{$aft_en}\">";
        $html .=     "<colgroup>";
        $html .=         "<col style=\"width:20%;\">";
        $html .=         "<col>";
        $html .=     "</colgroup>";
        $html .=     "<tr>";
        $html .=         "<th>{$aft_ko}</th>";
        $html .=         "<td>";
        $html .=             "<div></div>";
        $html .=             "<div class=\"toggle_wrapper\">";
        $html .=                 "<div id=\"{$prefix}_price_dd\" class=\"after_price\"></div>";
        $html .=                 "<div class=\"toggle\" action=\"aft\" aft=\"{$aft_en}\" dvs=\"{$dvs}\"></div>";
        $html .=                 "<input type=\"checkbox\" name=\"{$dvs}_chk_after[]\" id=\"${prefix}\" onclick=\"loadAfterPrice.exec(this.checked, this.value, '{$dvs}');\" value=\"{$aft_ko}\" aft=\"{$aft_en}\" class=\"aft_chkbox\" style=\"display:none;\" />";
        $html .=             "</div>";
        $html .=         "</td>";
        $html .=     "</tr>";

        $html .=     "<tr>";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<select id=\"{$prefix}_cnt\" onchange=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" class=\"basic_input\">";
        $html .=                 $cnt_option;
        $html .=             "</select>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 1줄
        $html .=     "<tr class=\"br _one\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($one_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_one_val1\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_one_val1\">중앙2단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($one_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_one_val2\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_one_val2\">비중앙2단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _one _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        $html .=                 "<tr>";
        if (isset($one_mpcode2)) {
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선  </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_one_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_one_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 2줄
        $html .=     "<tr class=\"br _two\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($two_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val1\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_two_val1\">비례3단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($two_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val2\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode2}\" dvs=\"M\"><label for=\"{$prefix}_two_val2\">십자4단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($two_mpcode3)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val3\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode3}\" dvs=\"C\"><label for=\"{$prefix}_two_val3\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _two _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($two_mpcode3)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_two_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_two_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_two_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_two_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 3줄
        $html .=     "<tr class=\"br _three\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($three_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_three_val1\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$three_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_three_val1\">비례4단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($three_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_three_val2\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$three_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_three_val2\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _three _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($three_mpcode2)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">세 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos3\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 4줄
        $html .=     "<tr class=\"br _four\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($four_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_four_val1\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$four_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_four_val1\">비례5단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($four_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_four_val2\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$four_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_four_val2\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _four _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($four_mpcode2)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">세 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos3\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">네 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos4\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_price\" name=\"{$prefix}_price\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_info\" name=\"{$prefix}_info\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_data\" name=\"{$prefix}_data\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_val\" name=\"{$prefix}_val\" value=\"\" />";
        $html .=         "</td>";
        $html .=     "</tr>";

        $html .= "</table>";
    }

    return $html;
}

/**
 * @brief 추가 후공정 미싱 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getDotlineInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "dotline";
    $aft_ko = "미싱";
    $prefix = $dvs . '_' . $aft_en;

    // depth1 기본값 처리
    $one_line   = '';
    $two_line   = '';
    $three_line = '';
    $four_line  = '';
    switch ($def["depth1"]) {
        case "1줄" :
            $one_line = "selected=\"selected\"";
            break;
        case "2줄" :
            $two_line = "selected=\"selected\"";
            break;
        case "3줄" :
            $three_line = "selected=\"selected\"";
            break;
        case "4줄" :
            $four_line = "selected=\"selected\"";
            break;
    }
    // depth2 기본값 처리
    $one_half = '';
    $one_n_half = '';

    $two_triple = '';
    $two_cross = '';
    $two_multi = '';

    $three_fourth = '';
    $three_multi = '';

    $four_fifth = '';
    $four_multi = '';

    if (!empty($one_line)) {
        switch ($def["depth2"]) {
            case "중앙2단" :
                $one_half = "checked=\"checked\"";
                break;
            case "비중앙2단" :
                $two_n_half = "checked=\"checked\"";
                break;
        }
    } else if (!empty($two_line)) {
        switch ($def["depth2"]) {
            case "비례3단" :
                $two_triple = "checked=\"checked\"";
                break;
            case "십자4단" :
                $two_cross = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $two_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($three_line)) {
        switch ($def["depth2"]) {
            case "비례4단" :
                $three_fourth = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $three_multi = "checked=\"checked\"";
                break;
        }
    } else if (!empty($four_line)) {
        switch ($def["depth2"]) {
            case "비례5단" :
                $four_fifth = "checked=\"checked\"";
                break;
            case "비대칭다단" :
                $four_multi = "checked=\"checked\"";
                break;
        }
    }
    // 위치값 기본값 처리
    $position_arr = [];
    if (!empty($def["info"])) {
        $position_arr = explode('|', $def["info"]);
    }

    $info_count = count($info);

    // html 생성하기 쉽도록 배열 가공
    $html_info = [];

    $option = "";
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];

        $mpcode = $temp["mpcode"];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];

        $html_info[$depth1][$depth2] = $mpcode;
    }

    $is_one   = false;
    $is_two   = false;
    $is_three = false;
    $is_four  = false;

    // 1줄
    $one_mpcode1 = $html_info["1줄"]["중앙"];
    $one_mpcode2 = $html_info["1줄"]["비중앙"];
    if (!empty($one_mpcode1) || !empty($one_mpcode_2)) {
        $is_one   = true;
    }
    // 2줄
    $two_mpcode1 = $html_info["2줄"]["비례3단"];
    $two_mpcode2 = $html_info["2줄"]["십자4단"];
    $two_mpcode3 = $html_info["2줄"]["비대칭다단"];
    if (!empty($two_mpcode1) ||
            !empty($two_mpcode_2) ||
            !empty($two_mpcode_3)) {
        $is_two   = true;
    }
    // 3줄
    $three_mpcode1 = $html_info["3줄"]["비례4단"];
    $three_mpcode2 = $html_info["3줄"]["비대칭다단"];
    if (!empty($three_mpcode1) || !empty($three_mpcode_2)) {
        $is_three = true;
    }
    // 4줄
    $four_mpcode1 = $html_info["4줄"]["비례5단"];
    $four_mpcode2 = $html_info["4줄"]["비대칭다단"];
    if (!empty($four_mpcode1) || !empty($four_mpcode_2)) {
        $is_four  = true;
    }

    $option = '';
    if ($is_one) {
        $cnt_option .= "<option value=\"one\" class=\"_one\" {$one_line}>1줄</option>";
    }
    if ($is_two) {
        $cnt_option .= "<option value=\"two\" class=\"_two\" {$two_line}>2줄</option>";
    }
    if ($is_three) {
        $cnt_option .= "<option value=\"three\" class=\"_three\" {$three_line}>3줄</option>";
    }
    if ($is_four) {
        $cnt_option .= "<option value=\"four\" class=\"_four\" {$four_line}>4줄</option>";
    }

    if (!$mobile_yn) {
        // 조건때문에 doc에서 str로 변경함
        $html  = "    <div class=\"option _{$aft_en}\">";
        $html .= "        <dl>";
        $html .= "            <dt>{$aft_ko}</dt>";
        $html .= "            <dd class=\"price\" id=\"{$prefix}_price_dd\"></dd>";
        $html .= "            <dd>";
        $html .= "                <select id=\"{$prefix}_cnt\" onchange=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" class=\"{$aft_en}_cnt\">";

        $html .=                      $cnt_option;
        $html .= "                </select>";
        $html .= "                <select id=\"{$prefix}_vh\" onchange=\"preview.{$aft_en}('{$dvs}');\">";
        $html .= "                    <option value=\"V\">세로</option>";
        $html .= "                    <option value=\"H\">가로</option>";
        $html .= "                </select>";
        $html .= "            </dd>";
        // 1줄
        $html .= "            <dd class=\"br _one\">";
        if (isset($one_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode1}\" dvs=\"M\" {$one_half}> 중앙2단</label>";
        }
        if (isset($one_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode2}\" class=\"_custom\" dvs=\"C\" {$one_n_half}> 비중앙2단</label>";
        }
        $html .= "            </dd>";
        if (isset($one_mpcode2)) {
            $html .= "            <dd class=\"br _one _custom\" >";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_one_pos1\" type=\"text\" class=\"mm {$prefix}_one_mm\" value=\"{$position_arr[0]}\">mm</label>";
            $html .= "            </dd>";
        }
        // 2줄
        $html .= "            <dd class=\"br _two\">";
        if (isset($two_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode1}\" dvs=\"M\" {$two_triple}> 비례3단</label>";
        }
        if (isset($two_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode2}\" dvs=\"M\" {$two_cross}> 십자4단</label>";
        }
        if (isset($two_mpcode3)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$two_mpcode3}\" dvs=\"C\" class=\"_custom\" {$two_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($two_mpcode3)) {
            $html .= "            <dd class=\"br _two _custom\">";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_two_pos1\" type=\"text\" class=\"mm {$prefix}_two_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_two_pos2\" type=\"text\" class=\"mm {$prefix}_two_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label>";
            $html .= "            </dd>";
        }
        // 3줄
        $html .= "            <dd class=\"br _three\">";
        if (isset($three_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$three_mpcode1}\" dvs=\"M\" {$three_fourth}> 비례4단</label>";
        }
        if (isset($three_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$three_mpcode2}\" dvs=\"C\" class=\"_custom\" {$three_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($three_mpcode2)) {
            $html .= "            <dd class=\"br _three _custom\" >";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_three_pos1\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_three_pos2\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_three_pos3\" type=\"text\" class=\"mm {$prefix}_three_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label>";
            $html .= "            </dd>";
        }

        // 4줄
        $html .= "            <dd class=\"br _four\">";
        if (isset($four_mpcode1)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$four_mpcode1}\" dvs=\"M\" {$four_fifth}> 비례5단</label>";
        }
        if (isset($four_mpcode2)) {
            $html .= "                <label><input type=\"radio\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$prefix}', '{$dvs}');\" value=\"{$four_mpcode2}\" dvs=\"C\" class=\"_custom\" {$four_multi}> 비대칭다단</label>";
        }
        $html .= "            </dd>";
        if (isset($four_mpcode2)) {
            $html .= "            <dd class=\"br _four _custom\" >";
            $html .= "                <label>첫 번째 선 <input id=\"{$prefix}_four_pos1\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[0]}\">mm</label><br />";
            $html .= "                <label>두 번째 선 <input id=\"{$prefix}_four_pos2\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[1]}\">mm</label><br />";
            $html .= "                <label>세 번째 선 <input id=\"{$prefix}_four_pos3\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[2]}\">mm</label><br />";
            $html .= "                <label>네 번째 선 <input id=\"{$prefix}_four_pos4\" type=\"text\" class=\"mm {$prefix}_four_mm\" onblur=\"chkAftRestrict('{$dvs}', '{$aft_en}');\" value=\"{$position_arr[3]}\">mm</label>";
            $html .= "            </dd>";
        }
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_price\" name=\"{$prefix}_price\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_info\" name=\"{$prefix}_info\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_data\" name=\"{$prefix}_data\" value=\"\" />";
        $html .= "            <input type=\"hidden\" id=\"{$prefix}_val\" name=\"{$prefix}_val\" value=\"\" />";
        $html .= "        </dl>";
        $html .= "    </div>";
    ///////////////////////////////////////////////////////
    } else {
    ///////////////////////////////////////////////////////
        $html .= "<table class=\"basic_table after_{$aft_en}\">";
        $html .=     "<colgroup>";
        $html .=         "<col style=\"width:20%;\">";
        $html .=         "<col>";
        $html .=     "</colgroup>";
        $html .=     "<tr>";
        $html .=         "<th>{$aft_ko}</th>";
        $html .=         "<td>";
        $html .=             "<div></div>";
        $html .=             "<div class=\"toggle_wrapper\">";
        $html .=                 "<div id=\"{$prefix}_price_dd\" class=\"after_price\"></div>";
        $html .=                 "<div class=\"toggle\" action=\"aft\" aft=\"{$aft_en}\" dvs=\"{$dvs}\"></div>";
        $html .=                 "<input type=\"checkbox\" name=\"{$dvs}_chk_after[]\" id=\"${prefix}\" onclick=\"loadAfterPrice.exec(this.checked, this.value, '{$dvs}');\" value=\"{$aft_ko}\" aft=\"{$aft_en}\" class=\"aft_chkbox\" style=\"display:none;\" />";
        $html .=                 "</div>";
        $html .=             "</div>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr>";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"basic_detail_table\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col style=\"width:45%;\">";
        $html .=                 "</colgroup>";
        $html .=                 "<tbody>";
        $html .=                     "<tr>";
        $html .=                         "<td style=\"padding-right:5% !important;\">";
        $html .=                             "<select id=\"{$prefix}_cnt\" onchange=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" class=\"basic_input Column_02\">";
        $html .=                                 $cnt_option;
        $html .=                             "</select>";
        $html .=                         "</td>";
        $html .=                         "<td>";
        $html .=                             "<select class=\"basic_input Column_02\" id=\"{$prefix}_vh\">";
        $html .=                                 "<option value=\"V\">세로</option>";
        $html .=                                 "<option value=\"H\">가로</option>";
        $html .=                             "</select>";
        $html .=                         "</td>";
        $html .=                     "</tr>";
        $html .=                 "</tbody>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 1줄
        $html .=     "<tr class=\"br _one\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($one_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_one_val1\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_one_val1\">중앙2단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($one_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_one_val2\" name=\"{$prefix}_one_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$one_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_one_val2\">비중앙2단.</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _one _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        $html .=                 "<tr>";
        if (isset($one_mpcode2)) {
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_one_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_one_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 2줄
        $html .=     "<tr class=\"br _two\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($two_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val1\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_two_val1\">비례3단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($two_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val2\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode2}\" dvs=\"M\"><label for=\"{$prefix}_two_val2\">십자4단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($two_mpcode3)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_two_val3\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$two_mpcode3}\" dvs=\"C\"><label for=\"{$prefix}_two_val3\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _two _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($two_mpcode3)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_two_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_two_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_two_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_two_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 3줄
        $html .=     "<tr class=\"br _three\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($three_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_three_val1\" name=\"{$prefix}_three_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$three_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_three_val1\">비례4단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($three_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_three_val2\" name=\"{$prefix}_two_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$three_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_three_val2\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _three _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($three_mpcode2)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">세 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_three_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_three_pos3\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        // 4줄
        $html .=     "<tr class=\"br _four\">";
        $html .=         "<td colspan=\"2\" style=\"padding:0 !important;\">";
        $html .=             "<table style=\"width:100%;\">";
        $html .=                 "<tr>";
        if (isset($four_mpcode1)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_four_val1\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$four_mpcode1}\" dvs=\"M\"><label for=\"{$prefix}_four_val1\">비례5단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        if (isset($four_mpcode2)) {
            $html .=                     "<td>";
            $html .=                         "<span class=\"after_option_span\">";
            $html .=                         "<input type=\"radio\" id=\"{$prefix}_four_val2\" name=\"{$prefix}_four_val\" onclick=\"getAfterPrice.common('{$aft_en}', '{$dvs}');\" value=\"{$four_mpcode2}\" dvs=\"C\"><label for=\"{$prefix}_four_val2\">비대칭다단</label>";
            $html .=                         "</span>";
            $html .=                     "</td>";
        }
        $html .=                 "</tr>";
        $html .=             "</table>";
        $html .=         "</td>";
        $html .=     "</tr>";
        $html .=     "<tr class=\"br _four _custom\">";
        $html .=         "<td colspan=\"2\">";
        $html .=             "<table class=\"order_size_form\">";
        $html .=                 "<colgroup>";
        $html .=                     "<col style=\"width:50%;\">";
        $html .=                     "<col>";
        $html .=                 "</colgroup>";
        if (isset($four_mpcode2)) {
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding:0 !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">첫 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos1\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">두 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos2\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">세 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos3\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
            $html .=                 "<tr>";
            $html .=                     "<td colspan=\"2\" style=\"padding-top:20px !important;\">";
            $html .=                         "<span class=\"order_size_form_text_after\">네 번째 선 </span>";
            $html .=                         "<input class=\"input_type_text_after_size {$prefix}_four_mm\" type=\"text\" maxlength=\"3\" readonly=\"readonly\" id=\"{$prefix}_four_pos4\">";
            $html .=                         "<span class=\"order_size_form_text_after\"> mm</span>";
            $html .=                     "</td>";
            $html .=                 "</tr>";
        }
        $html .=             "</table>";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_price\" name=\"{$prefix}_price\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_info\" name=\"{$prefix}_info\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_data\" name=\"{$prefix}_data\" value=\"\" />";
        $html .=             "<input type=\"hidden\" id=\"{$prefix}_val\" name=\"{$prefix}_val\" value=\"\" />";
        $html .=         "</td>";
        $html .=     "</tr>";

        $html .= "</table>";
    }

    return $html;
}

/**
 * @brief 추가 후공정 타공 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getPunchingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "punching";
    $aft_ko = "타공";
    $prefix = $dvs . '_' . $aft_en;

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

    $flag = true;
    $i = 1;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = "";

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $i++, $d1_attr);
    }

    // 기본값 위치처리
    $position_arr = [];

    $info_arr = explode('|', $def["info"]);
    foreach ($info_arr as $info) {
        $info = explode('*', $info);
        $position_arr[] = [
             "w" => $info[0]
            ,"v" => $info[1]
        ];
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select class="_num" id="{$prefix}_cnt" onchange="loadPunchingDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select name="{$prefix}_val" id="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                    </dd>
                    <dd class="br _on">
                        첫 번째 타공 위치
                        <label>가로 <input type="text" id="{$prefix}_pos_w1" class="mm" value="{$position_arr[0][w]}">mm</label>
                        <label>세로 <input type="text" id="{$prefix}_pos_h1" class="mm" value="{$position_arr[0][v]}">mm</label>
                    </dd>
                    <dd class="br">
                        두 번째 타공 위치
                        <label>가로 <input type="text" id="{$prefix}_pos_w2" class="mm" value="{$position_arr[1][w]}">mm</label>
                        <label>세로 <input type="text" id="{$prefix}_pos_h2" class="mm" value="{$position_arr[1][v]}">mm</label>
                    </dd>
                    <dd class="br">
                        세 번째 타공 위치
                        <label>가로 <input type="text" id="{$prefix}_pos_w3" class="mm" value="{$position_arr[2][w]}">mm</label>
                        <label>세로 <input type="text" id="{$prefix}_pos_h3" class="mm" value="{$position_arr[2][v]}">mm</label>
                     </dd>
                     <dd class="br">
                        네 번째 타공 위치
                        <label>가로 <input type="text" id="{$prefix}_pos_w4" class="mm" value="{$position_arr[3][w]}">mm</label>
                        <label>세로 <input type="text" id="{$prefix}_pos_h4" class="mm" value="{$position_arr[3][v]}">mm</label>
                    </dd>
                    <dd class="br">
                        다섯 번째 타공 위치
                        <label>가로 <input type="text" id="{$prefix}_pos_w5" class="mm" value="{$position_arr[4][w]}">mm</label>
                        <label>세로 <input type="text" id="{$prefix}_pos_h5" class="mm" value="{$position_arr[4][v]}">mm</label>
                    </dd>
                    <dd class="br note">
                        File에 타공 부분을 먹1도로 업로드 해주세요.
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                    <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_cnt" onchange="loadPunchingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" name="{$prefix}_val" id="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="order_size_form">
                                <colgroup>
                                    <col style="">
                                    <col style="">
                                </colgroup>
                                <tr class="br">
                                    <td colspan="2">
                                        <table class="basic_detail_table" style="margin:0 !important;">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="첫 번째 가로(mm)" id="{$prefix}_pos_w1">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" style="float:right;" type="text" maxlength="3" placeholder="첫 번째 세로(mm)" id="{$prefix}_pos_h1">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                                <tr class="br">
                                    <td colspan="2">
                                        <table class="basic_detail_table" style="margin-top: 20px !important;">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="두 번째 세로(mm)" id="{$prefix}_pos_w2">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" style="float:right;" type="text" maxlength="3" placeholder="두 번째 세로(mm)" id="{$prefix}_pos_h2">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                                <tr class="br">
                                    <td colspan="2">
                                        <table class="basic_detail_table" style="margin-top: 20px !important;">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세 번째 가로(mm)" id="{$prefix}_pos_w3">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" style="float:right;" type="text" maxlength="3" placeholder="세 번째 세로(mm)" id="{$prefix}_pos_h3">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                                <tr class="br">
                                    <td colspan="2">
                                        <table class="basic_detail_table" style="margin-top: 20px !important;">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="네 번째 가로(mm)" id="{$prefix}_pos_w4">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" style="float:right;" type="text" maxlength="3" placeholder="네 번째 세로(mm)" id="{$prefix}_pos_h4">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                                <tr class="br">
                                    <td colspan="2">
                                        <table class="basic_detail_table" style="margin-top: 20px !important;">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="다섯 번째 가로(mm)" id="{$prefix}_pos_w5">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" style="float:right;" type="text" maxlength="3" placeholder="다섯 번째 세로(mm)" id="{$prefix}_pos_h5">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <span class="order_after_note">* File에 타공 부분을 먹1도로 업로드 해주세요.</span>
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                        </td>
                    </tr>
                </table>
html;
    }


    return $html;
}

/**
 * @brief 추가 후공정 접지 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getFoldlineInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "foldline";
    $aft_ko = "접지";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2] = $mpcode;
    }

    $depth1_option = '';
    $depth2_option = '';

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        /* 240104 코드 추가 [시작] */
		if (array_key_exists("중앙", $depth2_arr)) {
			$tempArr = array("중앙" => $depth2_arr["중앙"]);
			unset($depth2_arr["중앙"]);
			$depth2_arr = $tempArr + $depth2_arr;
		}
		/* 240104 코드 추가 [종료] */
        $d1_attr = '';

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($depth2 === "비중앙") {
                    $d2_attr .= " class=\"_custom\"";
                }

                if ($mpcode === $def["mpcode"]) {
                    $d1_attr .= " selected=\"selected\"";
                    $d2_attr .= " selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }
        }

        if (strpos($depth1, "2단") !== false) {
            $d1_attr .= " col=\"2\"";
        } else if (strpos($depth1, "3단") !== false) {
            $d1_attr .= " col=\"3\"";
        } else if (strpos($depth1, "4단") !== false) {
            $d1_attr .= " col=\"4\"";
        } else if (strpos($depth1, "5단") !== false) {
            $d1_attr .= " col=\"5\"";
        } else if (strpos($depth1, "6단") !== false) {
            $d1_attr .= " col=\"6\"";
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_dvs" onchange="loadFoldlineDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('foldline', '{$dvs}');">
                            $depth2_option
                        </select>

                        <select id="{$prefix}_vh" onchange="preview.foldline('{$dvs}');">
                            <option value="V">세로</option>
                            <option value="H">가로</option>
                        </select>
                    </dd>
                    <dd class="br _col2">
                        <label>첫번째 선 <input type="text" class="mm" id="{$prefix}_pos" readonly="readonly" onblur="getAfterPrice.common('foldline', '{$dvs}');" value="{$def[info]}">mm</label>
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                    <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" onchange="loadFoldlineDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('foldline', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="order_size_form">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="">
                                </colgroup>
                                <tr>
                                    <td colspan="2" style="padding:0 !important;">
                                        <span class="order_size_form_text_after">첫 번째 선 </span>
                                        <input class="input_type_text_after_size" type="text" maxlength="3" readonly="readonly" id="{$prefix}_pos" onblur="getAfterPrice.common('foldline', '{$dvs}');">
                                        <span class="order_size_form_text_after"> mm</span>
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
 * @brief 추가 후공정 엠보싱 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getEmbossingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "embossing";
    $aft_ko = "엠보싱";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2] = $mpcode;
    }

    $depth1_option = '';
    $depth2_option = '';

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = '';

        if ($flag === true) {
            $flag = false;

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $depth2, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    $size_arr = explode('|', $def["info"]);
    $i = 0;
    $arr_count = count($size_arr);
    for ($i = 0; $i < $arr_count; $i++) {
        $size = $size_arr[$i];

        if (empty($size) || $size === "NaN") {
            $size = 0;
        }

        $size_arr[$i] = $size;
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_1" onchange="changeData();">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_dvs_1" name="{$prefix}_val" onchange="changeData();">
                            $depth2_option
                        </select>
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                    <dd class="br">
                        <label>가로 <input id="{$prefix}_wid_1" name="{$prefix}_wid_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[0]}">mm</label>
                        <label>세로 <input id="{$prefix}_vert_1" name="{$prefix}_vert_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[1]}">mm</label>
                    </dd>
                    <dd class="br note">
                        File에 박 부분을 먹1도로 업로드 해주세요.
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" onchange="loadEmbossingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="order_size_form">
                                <colgroup>
                                    <col style="">
                                    <col style="">
                                </colgroup>
                                <tr>
                                    <td colspan="2">
                                        <table class="basic_detail_table">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="가로(mm)" id="{$prefix}_wid_1" name="{$prefix}_wid_1" onblur="getAfterPrice.common('embossing', '{$dvs}');">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세로(mm)" id="{$prefix}_vert_1" name="{$prefix}_vert_1" onblur="getAfterPrice.common('embossing', '{$dvs}');">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                            </table>
                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
} 

/**
 * @brief 추가 후공정 박 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getFoilInfoHtml($info, $dvs, $mobile_yn, $def, $foil_count) {
    $aft_en = "foil";
    $aft_ko = "박";
    $prefix = $dvs . '_' . $aft_en;

    $foil_info = explode('|', $def["info"]);
    // 전면
    $bef_d1 = $foil_info[0];
    $bef_d2 = $foil_info[1];
    $bef_w = $foil_info[2];
    $bef_v = $foil_info[3];
    /*
    $aft_d1 = $foil_info[4];
    $aft_d2 = $foil_info[5];
    $aft_w = $foil_info[6];
    $aft_v = $foil_info[7];
    */

    $rs_price_html = number_format($foil_info[4]) . "원";
    $rs_price = $foil_info[4];
    $info_count = count($info);

    $dup_chk = [];

    $opt1 = '';
    $opt2 = '';

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2] = $mpcode;
    }

    $depth1_option = '';
    $depth2_option = '';

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = '';

        if ($flag === true) {
            $flag = false;

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $depth2, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    // 전면/후면/양면 선택값 처리
    $bef = '';
    $aft = '';
    $both = '';
    switch ($bef_d2) {
        case "단면" :
            $bef = "selected=\"selected\"";
            break;
        case "양면" :
            $aft = "selected=\"selected\"";
            break;
        case "양면다름" :
            $both = "selected=\"selected\"";
            break;
    }
    if (!empty($aft_d2)) {
        $aft = "selected=\"selected\"";
    }
    // 크기값 처리
    if (empty($bef_w) || $bef_w === "NaN") {
        $bef_w = 10;
    }
    if (empty($bef_v) || $bef_v === "NaN") {
        $bef_v = 10;
    }
    if (empty($aft_w) || $aft_w === "NaN") {
        $aft_w = 0;
    }
    if (empty($aft_v) || $aft_v === "NaN") {
        $aft_v = 0;
    }

    $foil_count_arr = explode("|", $foil_count); 

    if ($foil_count && $foil_count_arr[0] > 1) {
        $plus_btn = "<button type=\"button\" class=\"plus-btn\" id=\"plusBtn1\" style=\"display: none;\" onclick=\"addFoilSection(2, '" . $prefix . "', '" . $dvs . "');\">+</button>";
        $addFoilSection = "";
    } else {
        if($def["mpcode"] == "1302"){
            $plus_btn = "<button type=\"button\" class=\"plus-btn\" id=\"plusBtn1\" style=\"display: block;\" onclick=\"addFoilSection(2, '" . $prefix . "', '" . $dvs . "');\">+.</button>";
        }else{
             $plus_btn = "<button type=\"button\" class=\"plus-btn\" id=\"plusBtn1\" style=\"display: block;\" onclick=\"addFoilSection(2, '" . $prefix . "', '" . $dvs . "');\">+</button>";

        }
        $addFoilSection = "\n<div id=\"addFoilSection2\" class=\"option _foil _on\" style=\"display: block;\"></div>";
        $addFoilSection .= "\n<div id=\"addFoilSection3\" class=\"option _foil _on\" style=\"display: block;\"></div>";

    }

    if ($foil_count_arr[1] == 1 || !$foil_count) {
        if (!$mobile_yn) {
            $html = <<<html
                <div class="option _{$aft_en}">
                    <dl>
                        <dt>{$aft_ko}</dt>
                        <dd>
                            <select id="{$prefix}_1_1" name="{$prefix}_1_1"  style="width:85px;" class="_color" onchange="foilAreaInit('{$dvs}', this.value, '1_1');">
                                $depth1_option
                            </select>
                            <select id="{$prefix}_dvs_1_1" name="{$prefix}_dvs_1_1" style="min-width:60px;" onchange="changeFoilDvs('{$dvs}', this.value);">
                                <!--<option value="">-</option>-->
                                $depth2_option
                            </select>
                            <!-- 2021.9.19
                            &nbsp;/&nbsp;
                            <select id="{$prefix}_2" name="{$prefix}_2" style="width:85px;" onchange="foilAreaInit('{$dvs}', this.value, '2');">
                                <option value="">-</option>
                                $opt2
                            </select>
                            <select id="{$prefix}_dvs_2" name="{$prefix}_dvs_2" style="min-width:60px;" onchange="getAfterPrice.common('foil', '{$dvs}');">
                                <option value="">-</option>
                                <option value="후면" {$aft}>후면</option>
                            </select>
                            -->
                            <input type="hidden" id="{$prefix}_val" name="{$prefix}_val" value="{$def[mpcode]}" />
                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            <input type="hidden" id="{$prefix}_ec_price" name="{$prefix}_ec_price" value="" />
    
    
                            <input type="hidden" id="{$prefix}_name" name="{$prefix}_name" value="foil1" />
                            <input type="hidden" id="{$prefix}_depth" name="{$prefix}_depth" value="박" />
                            <input type="hidden" id="{$prefix}_1" name="{$prefix}_1" value="금박무광" />
                            <input type="hidden" id="{$prefix}_dvs_1" name="{$prefix}_dvs_1" value="양면동일" />
                            <input type="hidden" id="{$prefix}_wid_1" name="{$prefix}_wid_1" value="10" />
                            <input type="hidden" id="{$prefix}_vert_1" name="{$prefix}_vert_1" value="10" />
    
    			<!-- 2021.9.19
                        </dd>
                        <dd class="br">
    			-->
                            <label>가로 <input id="{$prefix}_wid_1_1" name="{$prefix}_wid_1_1" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$bef_w}">mm</label>
                            <label>세로 <input id="{$prefix}_vert_1_1" name="{$prefix}_vert_1_1" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$bef_v}">mm</label>
                            $plus_btn
                            
                            <!-- 2021.9.19
                            &nbsp;/&nbsp;
                            <label>가로 <input id="{$prefix}_wid_2" name="{$prefix}_wid_2" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$aft_w}">mm</label>
                            <label>세로 <input id="{$prefix}_vert_2" name="{$prefix}_vert_2" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$aft_v}">mm</label>
                            -->
                        </dd>
                        <dd class="br note" style="float:left; width:100%;">
                            File에 박 부분을 먹1도로 업로드 해주세요.
                        </dd>
                        <dd id="{$dvs}_foil1_price_dd" class="price">$rs_price_html</dd>
                        <input type="hidden" id="{$dvs}_foil1_price" name="{$dvs}_foil1_price" value="$rs_price" />
                    </dl>
                </div>
                $addFoilSection
html;
        } else {
            $html = <<<html
                        <table class="basic_table after_{$aft_en}">
                            <colgroup>
                                <col style="width:20%;">
                                <col style="">
                            </colgroup>
                            <tr>
                                <td>박</td>
                                <td>
                                    <div></div>
                                    <div class="toggle_wrapper">
                                        <div id="{$prefix}_price_dd" class="after_price"></div>
                                        <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                        <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <table class="basic_detail_table">
                                        <colgroup>
                                            <col style="width:50%;">
                                            <col style="width:45%;">
                                        </colgroup>
                                        <tbody>
                                            <tr>
                                                <td style="padding-right:5% !important;">
                                                    <select class="basic_input Column_02" id="{$prefix}_1" name="{$prefix}_1" onchange="foilAreaInit('{$dvs}', this.value, '1');">
                                                        <option value="">-</option>
                                                        $opt1
                                                    </select>
                                                </td>
                                                <td>
                                                    <select class="basic_input Column_02" id="{$prefix}_dvs_1" name="{$prefix}_dvs_1" onchange="changeFoilDvs('{$dvs}', this.value);">
                                                        <option value="">-</option>
                                                        <option value="전면" selected="selected">전면</option>
                                                        <option value="양면">양면</option>
                                                    </select>
                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <table class="order_size_form">
                                        <colgroup>
                                            <col style="">
                                            <col style="">
                                        </colgroup>
                                        <tr>
                                            <td colspan="2">
                                                <table class="basic_detail_table">
                                                    <colgroup>
                                                        <col style="width:50%;">
                                                        <col style="width:45%;">
                                                    </colgroup>
                                                    <tbody>
                                                        <tr>
                                                            <td style="padding-right:5% !important;">
                                                                <input class="basic_input Column_02" type="text" maxlength="3" placeholder="가로 사이즈(mm)" id="{$prefix}_wid_1" name="{$prefix}_wid_1" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" />
                                                            </td>
                                                            <td>
                                                                <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세로 사이즈(mm)" id="{$prefix}_vert_1" name="{$prefix}_vert_1" onblur="getAfterPrice.common('foil', '{$dvs}');" />
                                                            </td>
                                                        </tr>
                                                    </tbody>
                                                </table>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <table class="basic_detail_table">
                                        <colgroup>
                                            <col style="width:50%;">
                                            <col style="width:45%;">
                                        </colgroup>
                                        <tbody>
                                            <tr>
                                                <td style="padding-right:5% !important;">
                                                    <select class="basic_input Column_02" id="{$prefix}_2" name="{$prefix}_2" onchange="foilAreaInit('{$dvs}', this.value, '2');">
                                                        <option value="">-</option>
                                                        $opt2
                                                    </select>
                                                </td>
                                                <td>
                                                    <select class="basic_input Column_02" id="{$prefix}_dvs_2" name="{$prefix}_dvs_2" onchange="getAfterPrice.common('foil', '{$dvs}');">
                                                        <option value="">-</option>
                                                        <option value="전면">후면</option>
                                                    </select>
                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <table class="order_size_form">
                                        <colgroup>
                                            <col style="">
                                            <col style="">
                                        </colgroup>
                                        <tr>
                                            <td colspan="2">
                                                <table class="basic_detail_table">
                                                    <colgroup>
                                                        <col style="width:50%;">
                                                        <col style="width:45%;">
                                                    </colgroup>
                                                    <tbody>
                                                        <tr>
                                                            <td style="padding-right:5% !important;">
                                                                <input class="basic_input Column_02" type="text" maxlength="3" placeholder="가로 사이즈(mm)" id="{$prefix}_wid_2" name="{$prefix}_wid_2" onblur="getAfterPrice.common('foil', '{$dvs}');" />
                                                            </td>
                                                            <td>
                                                                <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세로 사이즈(mm)" id="{$prefix}_vert_2" name="{$prefix}_vert_2" onblur="getAfterPrice.common('foil', '{$dvs}');" />
                                                            </td>
                                                        </tr>
                                                    </tbody>
                                                </table>
                                            </td>
                                        </tr>
                                    </table>
    
                                    <input type="hidden" id="{$prefix}_val" name="{$prefix}_val" value="" />
                                    <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                                    <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <span class="order_after_note">* File에 박 부분을 먹1도로 업로드 해주세요.</span>
                                </td>
                            </tr>
                        </table>
html;
        }
    } else if ($foil_count_arr[1] > 1) {

        $foil_c = $foil_count_arr[1];

        $ctrl_btn = "";
        $addSection = "";
        if ($foil_count_arr[1] == 2) {
            $ctrl_btn = "<button type=\"button\" class=\"minus-btn\" id=\"minusBtn2\" onclick=\"removeFoilSection('2', '" . $dvs . "_foil', '" . $dvs . "');\">-</button>";
            $ctrl_btn .= "<button type=\"button\" class=\"plus-btn\" id=\"plusBtn2\" onclick=\"addFoilSection('3', '" . $dvs . "_foil', '" . $dvs . "');\">+</button>";
            $addSection .= "\n<div id=\"addFoilSection3\" class=\"option _foil _on\" style=\"display: block;\"></div>";
        } else if ($foil_count_arr[1] == 3) {
            $ctrl_btn = "<button type=\"button\" class=\"minus-btn\" id=\"minusBtn3\" onclick=\"removeFoilSection('3', '" . $dvs . "_foil', '" . $dvs . "');\">-</button>";
        }       

        $html = <<<html
            <div id="addFoilSection{$foil_c}" class="option _{$aft_en} _on" style="display: block;">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd>
                        <select id="{$prefix}_1_{$foil_c}" name="{$prefix}_1_{$foil_c}"  style="width:85px;" class="_color" onchange="foilAreaInit('{$dvs}', this.value, '1_{$foil_c}');">
                            $opt1
                        </select>
                        <select id="{$prefix}_dvs_1_{$foil_c}" name="{$prefix}_dvs_1_{$foil_c}" style="min-width:60px;" onchange="changeFoilDvs('{$dvs}', this.value);">
                            <!--<option value="">-</option>-->
                            <option value="단면" {$bef}>단면</option>
                            <option value="양면" {$aft}>양면</option>
                            <option value="양면다름" {$both}>양면다름</option>
                        </select>
                        <label>가로 <input id="{$prefix}_wid_1_{$foil_c}" name="{$prefix}_wid_1_{$foil_c}" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$bef_w}">mm</label>
                        <label>세로 <input id="{$prefix}_vert_1_{$foil_c}" name="{$prefix}_vert_1_{$foil_c}" type="text" class="mm" onblur="getAfterPrice.common('foil', '{$dvs}');" value="{$bef_v}">mm</label>
                        $ctrl_btn
                        
                    </dd>
                    <dd class="br note" style="float:left; width:100%;">
                        File에 박 부분을 먹1도로 업로드 해주세요.
                    </dd>
                    <dd id="{$dvs}_foil{$foil_c}_price_dd" class="price">$rs_price_html</dd>
                    <input type="hidden" id="{$dvs}_foil{$foil_c}_price" name="{$dvs}_foil{$foil_c}_price" value="$rs_price" />
                </dl>
            </div>
            $addSection
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 형압 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getPressInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "press";
    $aft_ko = "형압";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];

        $merge_arr[$depth1][$depth2] = $mpcode;
    }

    $depth1_option = '';
    $depth2_option = '';

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = '';

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }

        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    $size_arr = explode('|', $def["info"]);
    $i = 0;
    $arr_count = count($size_arr);
    for ($i = 0; $i < $arr_count; $i++) {
        $size = $size_arr[$i];

        if (empty($size) || $size === "NaN") {
            $size = 0;
        }

        $size_arr[$i] = $size;
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_1" name="{$prefix}_1" onchange="loadPressDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <!--select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select-->
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_val" name="{$prefix}_val" value="" />
                        <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                    <dd class="br">
                        <label>가로 <input id="{$prefix}_wid_1" name="{$prefix}_wid_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[0]}">mm</label>
                        <label>세로 <input id="{$prefix}_vert_1" name="{$prefix}_vert_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[1]}">mm</label>
                    </dd>
                    <dd class="br note">
                        File에 형압 부분을 먹1도로 업로드 해주세요.
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                    <table class="basic_table after_{$aft_en}">
                        <colgroup>
                            <col style="width:20%;">
                            <col style="">
                        </colgroup>
                        <tr>
                            <th>{$aft_ko}</th>
                            <td>
                                <div></div>
                                <div class="toggle_wrapper">
                                    <div id="{$prefix}_price_dd" class="after_price"></div>
                                    <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                    <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <table class="basic_detail_table">
                                    <colgroup>
                                        <col style="width:50%;">
                                        <col style="width:45%;">
                                    </colgroup>
                                    <tbody>
                                        <tr>
                                            <td style="padding-right:5% !important;">
                                                <select class="basic_input Column_02" id="{$prefix}_1" name="{$prefix}_1" onchange="loadPressDepth2(this.value, '{$dvs}');">
                                                    $depth1_option
                                                </select>
                                            </td>
                                            <td>
                                                <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                    $depth2_option
                                                </select>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <table class="order_size_form">
                                    <colgroup>
                                        <col style="">
                                        <col style="">
                                    </colgroup>
                                    <tr>
                                        <td colspan="2">
                                            <table class="basic_detail_table">
                                                <colgroup>
                                                    <col style="width:50%;">
                                                    <col style="width:45%;">
                                                </colgroup>
                                                <tbody>
                                                    <tr>
                                                        <td style="padding-right:5% !important;">
                                                            <input class="basic_input Column_02" type="text" maxlength="3" placeholder="가로 사이즈(mm)" id="{$dvs}_press_wid_1" name="{$dvs}_press_wid_1" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" />
                                                        </td>
                                                        <td>
                                                            <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세로 사이즈(mm)" id="{$prefix}_vert_1" name="{$prefix}_vert_1" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" />
                                                        </td>
                                                    </tr>
                                                </tbody>
                                            </table>
                                        </td>
                                    </tr>
                                </table>

                                <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                                <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <span class="order_after_note">* File에 형압 부분을 먹1도로 업로드 해주세요.</span>
                            </td>
                        </tr>
                    </table>
html;
    }


    return $html;
}

/**
 * @brief 추가 후공정 도무송 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getTomsonInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "thomson";
    $aft_ko = "도무송";
    $prefix = $dvs . '_' . $aft_en;

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

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = "";

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $depth2, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    $size_arr = explode('|', $def["info"]);

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_dvs" onchange="loadThomsonDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_dvs_1" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                        <select id="{$prefix}_cnt" name="{$prefix}_cnt" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            <option value="1">1개</option>
                            <option value="2">2개</option>
                            <option value="3">3개</option>
                            <option value="4">4개</option>
                            <option value="5">5개</option>
                            <option value="6">6개</option>
                        </select>
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                    <dd class="br">
                        <label>가로 <input id="{$prefix}_wid_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[0]}">mm</label>
                        <label>세로 <input id="{$prefix}_vert_1" type="text" class="mm" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');" value="{$size_arr[1]}">mm</label>
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" onchange="loadThomsonDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="order_size_form">
                                <colgroup>
                                    <col style="">
                                    <col style="">
                                </colgroup>
                                <tr>
                                    <td colspan="2">
                                        <table class="basic_detail_table">
                                            <colgroup>
                                                <col style="width:50%;">
                                                <col style="width:45%;">
                                            </colgroup>
                                            <tbody>
                                                <tr>
                                                    <td style="padding-right:5% !important;">
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="가로 사이즈(mm)" id="{$prefix}_wid" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                    </td>
                                                    <td>
                                                        <input class="basic_input Column_02" type="text" maxlength="3" placeholder="세로 사이즈(mm)" id="{$prefix}_vert" onblur="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                    </td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </td>
                                </tr>
                            </table>

                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 넘버링 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getNumberingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "numbering";
    $aft_ko = "넘버링";
    $prefix = $dvs . '_' . $aft_en;

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

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = "";

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $attr, $d1_attr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_dvs" onchange="loadDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" onchange="loadDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>

                                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 재단 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getCuttingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "cutting";
    $aft_ko = "재단";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $option = '';
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $attr = '';

        switch ($temp["depth1"]) {
        case "2등분":
            $attr = "num=\"2\"";
            break;
        case "3등분":
            $attr = "num=\"3\"";
            break;
        case "4등분":
            $attr = "num=\"4\"";
            break;
        case "5등분":
            $attr = "num=\"5\"";
            break;
        case "6등분":
            $attr = "num=\"6\"";
            break;
        case "7등분":
            $attr = "num=\"7\"";
            break;
        case "8등분이상":
            $attr = "num=\"8\"";
            break;
        case "스티커 추가재단":
            $attr = "num=\"sticker\"";
            break;
        case "정규격재단":
            $attr = "num=\"sticker\"";
            break;
        case "작업선제거재단":
            $attr = "num=\"sticker2\"";
            break;
        default:
            $attr = "num=\"label\"";
            break;
        }

        if ($temp["mpcode"] === $def["mpcode"]) {
            $attr = " selected=\"selected\"";
        }

        $option .= option($temp["depth1"], $temp["mpcode"], $attr);
    }

    if (!$mobile_yn) {
        if($temp["depth1"] == "정규격재단") {
            $html = <<<html
            <div class="option _{$aft_en}" style="position:relative;">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            {$option}
                        </select>

                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                    </dd>
                    <dd>
                        <label>가로 <input type="text" class="mm" id="{$prefix}_wid" onblur="changeData();"></label>
                        <label>세로 <input type="text" class="mm" id="{$prefix}_vert" onblur="changeData();"></label>
                    </dd>
                    <dt>{$aft_ko}</dt>
                    <dd>
                        <p class="note">4cm 미만은 재단이 불가능합니다.</p>
                    </dd>
                </dl>
            </div>
html;
        } else {
            $html = <<<html
            <div class="option _{$aft_en}" style="position:relative;">
                <dl>
                    <dt></dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            {$option}
                        </select>

                        <select id="{$prefix}_vh" onchange="preview.{$aft_en}('{$dvs}');" style="display: none;">
                            <option value="V">세로</option>
                            <option value="H">가로</option>
                        </select>

                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                    </dd>
                    <!--dd class="br">
                        <label>가로 <input type="text" class="mm" id="{$prefix}_wid"></label>
                        <label>세로 <input type="text" class="mm" id="{$prefix}_vert"></label>
                        <label><input type="text" class="mm"  id="{$prefix}_div"> 등분</label>
					    <img class="icon_cutting_label" src="/design_template/images/common/icon_tip02.png" alt="tip">
                    </dd-->
                    <dt>{$aft_ko}</dt>
                    <dd>
                        <p class="note">4cm 미만은 재단이 불가능합니다.</p>
                    </dd>
                </dl>
            </div>
html;
        }
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <select class="basic_input" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                {$option}
                            </select>
                        </td>
                    </tr>
                    <tr class="cutting_label">
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <input class="basic_input Column_02" type="text" maxlength="2" placeholder="가로등분" id="{$prefix}_wid">
                                        </td>
                                        <td>
                                            <input class="basic_input Column_02" type="text" maxlength="2" placeholder="세로등분"id="{$prefix}_vert">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 제본 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getBindingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "binding";
    $aft_ko = "제본";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $merge_arr = [];

    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];
        $depth1 = $temp["depth1"];
        $depth2 = $temp["depth2"];
        $mpcode = $temp["mpcode"];
        $merge_arr[$depth1][$depth2]["depth2"] = $depth2;
        $merge_arr[$depth1][$depth2]["mpcode"] = $mpcode;
        //$merge_arr[$depth1][$mpcode] = $mpcode;
    }

    $depth1_option = "";
    $depth2_option = "";

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = "";
        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\" name=\"true\"";
                }

                $depth2_option .= option($depth2["depth2"], $depth2["mpcode"], $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_dvs" onchange="loadBindingDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('binding', '{$dvs}');">
                            $depth2_option
                        </select>
                        <select id="{$prefix}_bind_amt" onchange="preview.foldline('{$dvs}');">
                            <option value="100">100매철</option>
                            <!--option value="200">200매철</option-->
                        </select>
                    </dd>
                    <dd class="br note">
                            ＊ 50매철 및 별도매철 원하시는경우 분리없이 보내드리며, 
                            <br>직접 떼어내어 사용하셔야합니다.<br>
                            ＊ 아트/스노우 용지는 제본시 잘 떨어질 수 있으니
                            <br>이점 고려하셔서 선택해주시기 바랍니다.
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" onchange="loadBindingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>

                                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>

                                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 접착 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getBondingInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "bonding";
    $aft_ko = "접착";
    $prefix = $dvs . '_' . $aft_en;

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

    $flag = true;
    foreach ($merge_arr as $depth1 => $depth2_arr) {
        $d1_attr = "";

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }
        }

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_depth1" onchange="loadBondingDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" style="width:200px;" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_depth1" onchange="loadBondingDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>

                                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 라미넥스 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getLaminexInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "laminex";
    $aft_ko = "라미넥스";
    $prefix = $dvs . '_' . $aft_en;

    $option = option($aft_ko, $info[0]["mpcode"]);

    $amt = $def["info"];
    if (empty($amt) || $amt === "NaN" || intval($amt) < 10) {
        $amt = 10;
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val">
                            $option
                        </select>
                        <label><input type="text" id="{$dvs}_laminex_cnt" name="{$dvs}_laminex_cnt" onblur="changeData();" class="page" value="{$amt}"> 매</label>
                        <p class="note">최소 <span id="{$dvs}_laminex_min">10</span>장, 최대 <span id="{$dvs}_laminex_max">1,000</span>장까지 가능합니다.</p>
                    </dd>
                    <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                    <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val">
                                                $option
                                            </select>
                                        </td>
                                        <td>
                                            <input class="basic_input Column_02" type="text" maxlength="4"  id="{$dvs}_laminex_amt" name="{$dvs}_laminex_amt" onblur="changeData();" value="10">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <span class="order_after_note">* 최소 <span id="{$prefix}_min">10</span>장, 최대 1,000장까지 가능합니다.</span>

                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                            <input type="hidden" id="{$prefix}_data" name="{$prefix}_data" value="" />
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 가공 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getManufactureInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "manufacture";
    $aft_ko = "가공";
    $prefix = $dvs . '_' . $aft_en;

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
        $d1_attr = "";

        if ($flag === true) {
            if (empty($def) || $depth1 === $def["depth1"]) {
                $flag = false;
            }

            foreach ($depth2_arr as $depth2 => $mpcode) {
                $d2_attr = '';
                if ($mpcode === $def["mpcode"]) {
                    $d1_attr = "selected=\"selected\"";
                    $d2_attr = "selected=\"selected\"";
                }

                $depth2_option .= option($depth2, $mpcode, $d2_attr);
            }
        }

        //if ($depth1 === "끈작업") {
        //    $line_manu = true;
        //}

        $depth1_option .= option($depth1, $depth1, $d1_attr);
    }

    $line_manu_note = '';
    //if ($line_manu) {
    //    $line_manu_note = "끈작업은 최대 40,000장 까지 가능합니다.";
    //}

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_dvs" name="{$prefix}_dvs" onchange="loadManufactureDepth2(this.value, '{$dvs}');">
                            $depth1_option
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth2_option
                        </select>
                        <!--
                        <p class="note">
                            $line_manu_note
                        </p>
                        -->
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_dvs" name="{$prefix}_dvs" onchange="loadManufactureDepth2(this.value, '{$dvs}');">
                                                $depth1_option
                                            </select>
                                        </td>
                                        <td>
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $depth2_option
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <span class="order_after_note note">
                                $line_manu_note
                            </span>
                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 복권 실크 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getLotterySilkInfoHtml($info, $dvs, $mobile_yn, $def) {
    $aft_en = "lotterysilk";
    $aft_ko = "복권실크";
    $prefix = $dvs . '_' . $aft_en;

    $option = option("복권실크", $info["mpcode"]);

    $amt = $def["info"];
    if (empty($amt) || $amt === "NaN") {
        $amt = 0;
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd id="{$prefix}_price_dd" class="price"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $option
                        </select>
                        <label><input type="text" id="{$prefix}_info" name="{$prefix}_info" class="page" value="{$amt}" /> 매</label>
                    </dd>
                    <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table after_{$aft_en}">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$aft_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price"></div>
                                <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                                                $option
                                            </select>
                                        </td>
                                        <td>
                                            <input class="basic_input Column_02" type="text" maxlength="3" placeholder="매수를 입력해주세요." id="{$prefix}_info" name="{$prefix}_info">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </table>
html;
    }

    return $html;
}

/******************************************************************************
 ***************************** 이하 실사주문 관련 함수
 ******************************************************************************/

/**
 * @brief 실사출력 기본후공정 html 생성
 *
 * @detail 후공정에 따라서 부가정보 출력이 틀려짐
 *
 * @param $dvs      = 제품 구분값
 * @param $name_ko  = 후공정 한글명
 * @param $name_en  = 후공정 영문명
 * @param $info     = 후공정 정보배열
 *
 * @return 실사 기본후공정 html
 */
function getAoBasicAfterHtml($dvs, $name_ko, $name_en, $info, $mobile_yn) {
    $prefix = $dvs . '_' . $name_en;
    $func_name = sprintf("getAoAfterPrice.common('%s', '%s')", $name_en, $dvs);
    $style_extra = "width:10%;";

    // select box 모양
    $select_form = "<select id=\"%s\" name=\"%s\" style=\"width:20%%;\" onchange=\"%s;\">";
    if ($mobile_yn) {
        $select_form = "<select class=\"basic_input Column_02\" id=\"%s\" name=\"%s\" onchange=\"%s;\">";
    }

    // checkbox 모양
    $posi_chk = <<<chk_html
        <span>
            <span style="font-weight:bold; margin-left:10px; margin-right:5px;">위치</span>
            <span><label class="check"><input type="checkbox" id="{$prefix}_t" name="{$prefix}_dvs" value="상" checked="checked" onclick="{$func_name}" /><i><i></i></i>상</span></label>
            <span><label class="check"><input type="checkbox" id="{$prefix}_b" name="{$prefix}_dvs" value="하" checked="checked" onclick="{$func_name}" /><i><i></i></i>하</span></label>
            <span><label class="check"><input type="checkbox" id="{$prefix}_l" name="{$prefix}_dvs" value="좌" checked="checked" onclick="{$func_name}" /><i><i></i></i>좌</span></label>
            <span><label class="check"><input type="checkbox" id="{$prefix}_r" name="{$prefix}_dvs" value="우" checked="checked" onclick="{$func_name}" /><i><i></i></i>우</span></label>
        </span>
chk_html;
    if ($mobile_yn) {
        $posi_chk = <<<chk_html
                    <tr>
                        <td colspan="2" style="padding: 0 !important;">
                            <table style="width:100%;">
                                <colgroup>
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                    <col style="width:25%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <span class="after_option_span">
                                                <input type="checkbox" id="${prefix}_t" name="{$prefix}_dvs" value="상" onclick="{$func_name}"><label for="${prefix}_t">상</label>
                                            </span>
                                        </td>
                                        <td>
                                            <span class="after_option_span">
                                                <input type="checkbox" id="${prefix}_b" name="{$prefix}_dvs" value="하" onclick="{$func_name}"><label for="${prefix}_b">하</label>
                                            </span>
                                        </td>
                                        <td>
                                            <span class="after_option_span">
                                                <input type="checkbox" id="${prefix}_l" name="{$prefix}_dvs" value="좌" onclick="{$func_name}"><label for="${prefix}_l">좌</label>
                                            </span>
                                        </td>
                                        <td>
                                            <span class="after_option_span">
                                                <input type="checkbox" id="${prefix}_r" name="{$prefix}_dvs" value="우" onclick="{$func_name}"><label for="${prefix}_r">우</label>
                                            </span>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
chk_html;
    }

    $count_option = '';
    for ($i = 1; $i <= 50; $i++) {
        $count_option .= option($i . '개', $i);
    }
    $posi_count = <<<count_html
        <label>
            <select id="{$prefix}_t" name="{$prefix}_dvs" onchange="{$func_name}" style="{$style_extra}">
                <option value="-1">상</option>
                {$count_option}
            </select>
        </label>
        <label>
            <select id="{$prefix}_b" name="{$prefix}_dvs" onchange="{$func_name}" style="{$style_extra}">
                <option value="-1">하</option>
                {$count_option}
            </select>
        </label>
        <label>
            <select id="{$prefix}_l" name="{$prefix}_dvs" onchange="{$func_name}" style="{$style_extra}">
                <option value="-1">좌</option>
                {$count_option}
            </select>
        </label>
        <label>
            <select id="{$prefix}_r" name="{$prefix}_dvs" onchange="{$func_name}" style="{$style_extra}">
                <option value="-1">우</option>
                {$count_option}
            </select>
        </label>
count_html;
    if ($mobile_yn) {
        $posi_count = <<<count_html
                    <tr>
                        <td colspan="2">
                            <select class="basic_input Column_02" id="{$prefix}_t" name="{$prefix}_dvs" onchange="{$func_name}">
                                <option value="-1">상</option>
                                {$count_option}
                            </select>
                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_b" name="{$prefix}_dvs" onchange="{$func_name}">
                                <option value="-1">하</option>
                                {$count_option}
                            </select>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <select class="basic_input Column_02" id="{$prefix}_l" name="{$prefix}_dvs" onchange="{$func_name}">
                                <option value="-1">좌</option>
                                {$count_option}
                            </select>
                            <select class="basic_input Column_02" style="float:right;" id="{$prefix}_r" name="{$prefix}_dvs" onchange="{$func_name}">
                                <option value="-1">우</option>
                                {$count_option}
                            </select>
                        </td>
                    </tr>
count_html;
    }

    // 사방선택 등 여부 확인
    $depth1 = '';
    $depth2 = '';
    $attr   = '';
    switch ($name_ko) {
        case "열재단" :
            $depth1 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            foreach ($info as $i) {
                $depth1 .= option($i["depth1"], $i["mpcode"]);
            }
            $depth1 .= "</select>";

            $attr = $posi_chk;

            break;
        case "미싱" :
            $depth1 = sprintf($select_form, $prefix . "_depth1"
                                          , $prefix . "_depth1"
                                          , "loadAoDotlineDepth2('" . $name_en . "', this.value)");
            $depth2 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            $depth1_name = '';
            $dup_chk = [];
            foreach ($info as $i) {
                if (empty($dup_chk[$i["depth1"]])) {
                    $dup_chk[$i["depth1"]] = true;
                    $depth1 .= option($i["depth1"], $i["depth1"]);
                }

                if (empty($depth1_name)) {
                    $depth1_name = $i["depth1"];
                }

                if ($depth1_name === $i["depth1"]) {
                    $depth2 .= option($i["depth2"], $i["mpcode"]);
                }
            }
            $depth1 .= "</select>";
            $depth2 .= "</select>";

            $attr = $posi_chk;

            break;
        case "고리" :
            // 추가됨
            $style_extra = "width:10%;";
            $func_name = "changeAoAfterRing('%s', '%s', this)";
            $func_name = sprintf($func_name, $name_en, $dvs);
            $depth1 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            foreach ($info as $i) {
                $depth1 .= option($i["depth1"], $i["mpcode"]);
            }
            $depth1 .= "</select>";

            $attr = $posi_count;

            break;
        case "족자" :
            $depth1 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            foreach ($info as $i) {
                $depth1 .= option($i["depth1"], $i["mpcode"]);
            }
            $depth1 .= "</select>";

            break;
        case "쿨코팅" :
            $depth1 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            foreach ($info as $i) {
                $depth1 .= option($i["depth1"], $i["mpcode"]);
            }
            $depth1 .= "</select>";

            break;
        case "타공" :
            $depth1 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            foreach ($info as $i) {
                $depth1 .= option($i["depth1"], $i["mpcode"]);
            }
            $depth1 .= "</select>";

            break;
        case "재단" :
            $depth1 = sprintf($select_form, $prefix . "_depth1"
                                          , $prefix . "_depth1"
                                          , "loadAoCuttingDepth2('" . $name_en . "', this.value)");
            $depth2 = sprintf($select_form, $prefix . "_val"
                                          , $prefix . "_val"
                                          , $func_name);
            $depth1_name = '';
            $dup_chk = [];
            foreach ($info as $i) {
                if (empty($dup_chk[$i["depth1"]])) {
                    $dup_chk[$i["depth1"]] = true;
                    $depth1 .= option($i["depth1"], $i["depth1"]);
                }

                if (empty($depth1_name)) {
                    $depth1_name = $i["depth1"];
                }

                if ($depth1_name === $i["depth1"]) {
                    $depth2 .= option($i["depth2"] . " 여백", $i["mpcode"]);
                }
            }
            $depth1 .= "</select>";
            $depth2 .= "</select>";

            break;
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="ao_basic_after">
                <div style="width:43px;float:left;padding:1px 0 0 0;">{$name_ko}</div>
                {$depth1}
                {$depth2}
                {$attr}
                <span id="{$prefix}_price_dd" style="width:90px;text-align:right;font-weight:bold;float:right;padding:1px 0 0 0;">0원</span>
                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" class="ao_basic_after_price" value="0" />
                <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" />
                <input type="hidden" id="{$prefix}_mpcode" name="ao_basic_after_mpcode[]" value="" />
                <input type="checkbox" id="{$prefix}" name="{$dvs}_chk_after[]" value="{$name_ko}" aft="{$name_en}" checked="checked" style="display:none;" />
            </div>
html;
    } else {
        $html = <<<html
                <table class="basic_table ao_basic_after">
                    <colgroup>
                        <col style="width:20%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>{$name_ko}</th>
                        <td>
                            <div></div>
                            <div class="toggle_wrapper">
                                <div id="{$prefix}_price_dd" class="after_price">0원</div>
                            </div>

                            <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" class="ao_basic_after_price" value="0" />
                            <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" />
                            <input type="hidden" id="{$prefix}_mpcode" name="ao_basic_after_mpcode[]" value="" />
                            <input type="checkbox" id="{$prefix}" name="{$dvs}_chk_after[]" value="{$name_ko}" aft="{$name_en}" checked="checked" style="display:none;" />
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            {$depth1}
                                        </td>
                                        <td>
                                            {$depth2}
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    {$attr}
                </table>
html;
    }

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 열재단 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoHeatCuttingHtml($info, $dvs) {
    $name_en = "cutting_heat";
    $depth1 = '';

    foreach ($info as $i) {
        $depth1 .= option($i["depth1"], $i["mpcode"]);
    }

    $html = <<<html
        <div class="option _{$name_en}">
            <dl>
                <dt>열재단</dt>
                <dd class="price" id="{$dvs}_{$name_en}_price_dd"></dd>
                <dd>
                    <select id="{$dvs}_{$name_en}_val" name="{$dvs}_{$name_en}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                        $depth1
                    </select>
                </dd>
                <dd class="br">
                    <label><input name="{$dvs}_{$name_en}_dvs" value="상" type="checkbox" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');"> 상</label>
                    <label><input name="{$dvs}_{$name_en}_dvs" value="하" type="checkbox" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');"> 하</label>
                    <label><input name="{$dvs}_{$name_en}_dvs" value="좌" type="checkbox" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');"> 좌</label>
                    <label><input name="{$dvs}_{$name_en}_dvs" value="우" type="checkbox" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');"> 우</label>
                    <input type="hidden" id="{$dvs}_{$name_en}_info" name="{$dvs}_{$name_en}_info" value="" />
                    <input type="hidden" id="{$dvs}_{$name_en}_price" name="{$dvs}_{$name_en}_price" value="" />
                </dd>
            </dl>
        </div>
html;

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 미싱 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoDotlineHtml($info, $dvs, $name_en, $mobile_yn) {
    $name_ko = "추가미싱";
    $prefix = $dvs . '_' . $name_en;

    $depth1 = '';
    $depth2 = '';

    $depth1_name = '';
    $dup_chk = [];
    foreach ($info as $i) {
        if (empty($dup_chk[$i["depth1"]])) {
            $dup_chk[$i["depth1"]] = true;
        } else {
            continue;
        }

        $depth1 .= option($i["depth1"], $i["depth1"]);

        if (empty($depth1_name)) {
            $depth1_name = $i["depth1"];
        }

        if ($depth1_name === $i["depth1"]) {
            $depth2 .= option($i["depth2"], $i["mpcode"]);
        }
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$name_en}">
                <dl>
                    <dt>{$name_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_depth1" name="{$prefix}_depth1" onchange="loadAoDotlineDepth2('{$name_en}', this.value);">
                            {$depth1}
                        </select>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                            {$depth2}
                        </select>
                    </dd>
                    <dd class="br">
                        <label class="check"><input id="{$prefix}_t" name="{$prefix}_dvs" value="상" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><i><i></i></i> 상</label>
                        <label class="check"><input id="{$prefix}_b" name="{$prefix}_dvs" value="하" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><i><i></i></i> 하</label>
                        <label class="check"><input id="{$prefix}_l" name="{$prefix}_dvs" value="좌" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><i><i></i></i> 좌</label>
                        <label class="check"><input id="{$prefix}_r" name="{$prefix}_dvs" value="우" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><i><i></i></i> 우</label>
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
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
                    <th>{$name_ko}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div id="{$prefix}_price_dd" class="after_price"></div>
                            <div class="toggle" action="aft" aft="{$name_en}" dvs="{$dvs}"></div>
                            <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAoAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$name_ko}" aft="{$name_en}" class="aft_chkbox" style="display:none;" />
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:50%;">
                                <col style="width:45%;">
                            </colgroup>
                            <tbody>
                                <tr>
                                    <td style="padding-right:5% !important;">
                                        <select class="basic_input Column_02" id="{$prefix}_depth1" name="{$prefix}_depth1" onchange="loadAoDotlineDepth2('{$name_en}', this.value);">
                                            {$depth1}
                                        </select>
                                    </td>
                                    <td>
                                        <select class="basic_input Column_02" id="{$prefix}_val" name="{$prefix}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                                            {$depth2}
                                        </select>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" style="padding: 0 !important;">
                        <table style="width:100%;">
                            <colgroup>
                                <col style="width:25%;">
                                <col style="width:25%;">
                                <col style="width:25%;">
                                <col style="width:25%;">
                            </colgroup>
                            <tbody>
                            <tr>
                                <td>
                                    <span class="after_option_span">
                                        <label class="check"><input id="{$prefix}_t" name="{$prefix}_dvs" value="상" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><i><i></i></i><label for="{$prefix}_t">상</label></label>
                                    </span>
                                </td>
                                <td>
                                    <span class="after_option_span">
                                        <input id="{$prefix}_b" name="{$prefix}_dvs" value="하" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><label for="{$prefix}_b">하</label>
                                    </span>
                                </td>
                                <td>
                                    <span class="after_option_span">
                                        <input id="{$prefix}_l" name="{$prefix}_dvs" value="좌" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><label for="{$prefix}_l">좌</label>
                                    </span>
                                </td>
                                <td>
                                    <span class="after_option_span">
                                        <input id="{$prefix}_r" name="{$prefix}_dvs" value="우" type="checkbox" onclick="getAoAfterPrice.common('{$name_en}', '{$dvs}');"><label for="{$prefix}_r">우</label>
                                    </span>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </table>
html;
    }

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 고리 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoRingHtml($info, $dvs, $mobile_yn) {
    $aft_en = "ring";
    $aft_ko = "고리";
    $prefix = $dvs . '_' . $aft_en;

    $depth1 = '';

    $func_name = "getAoAfterPrice.common('%s', '%s');";
    $func_name = sprintf($func_name, $aft_en, $dvs);

    $count_option = '';
    for ($i = 1; $i <=50; $i++) {
        $count_option .= option($i . '개', $i);
    }

    if (!$mobile_yn) {
        /*
        $posi_count = <<<count_html
            <label>
                <select id="{$prefix}_t" name="{$prefix}_dvs" onchange="{$func_name}">
                    <option value="-1">상</option>
                    {$count_option}
                </select>
            </label>
            <label>
                <select id="{$prefix}_b" name="{$prefix}_dvs" onchange="{$func_name}">
                    <option value="-1">하</option>
                    {$count_option}
                </select>
            </label>
            <label>
                <select id="{$prefix}_l" name="{$prefix}_dvs" onchange="{$func_name}">
                    <option value="-1">좌</option>
                    {$count_option}
                </select>
            </label>
            <label>
                <select id="{$prefix}_r" name="{$prefix}_dvs" onchange="{$func_name}">
                    <option value="-1">우</option>
                    {$count_option}
                </select>
            </label>
count_html;
        */
    } else {
        $posi_count = <<<count_html
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:50%;">
                                <col style="width:45%;">
                            </colgroup>
                            <tbody>
                                <tr>
                                    <td style="padding-right:5% !important;">
                                        <select class="basic_input Column_02" id="{$prefix}_t" name="{$prefix}_dvs" onchange="{$func_name}">
                                            <option value="-1">상</option>
                                            {$count_option}
                                        </select>
                                    </td>
                                    <td>
                                        <select class="basic_input Column_02" style="float:right;" id="{$prefix}_b" name="{$prefix}_dvs" onchange="{$func_name}">
                                            <option value="-1">하</option>
                                            {$count_option}
                                        </select>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:50%;">
                                <col style="width:45%;">
                            </colgroup>
                            <tbody>
                                <tr>
                                    <td style="padding-right:5% !important;">
                                        <select class="basic_input Column_02" id="{$prefix}_l" name="{$prefix}_dvs" onchange="{$func_name}">
                                            <option value="-1">좌</option>
                                            {$count_option}
                                        </select>
                                    </td>
                                    <td>
                                        <select class="basic_input Column_02" style="float:right;" id="{$prefix}_r" name="{$prefix}_dvs" onchange="{$func_name}">
                                            <option value="-1">우</option>
                                            {$count_option}
                                        </select>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
count_html;
    }

    foreach ($info as $i) {
        // 상민 : 20240320 사방쇠고리 작업
        // 아래 2개줄 추가 및 3번째 줄 checked 옵션 추가
        $checked = "";
        if($i["depth1"]==="사방쇠고리") $checked = "selected";
        $depth1 .= option($i["depth1"], $i["mpcode"], $checked);
    }

    $func_name = "changeAoAfterRing('%s', '%s', this);";
    $func_name = sprintf($func_name, $aft_en, $dvs);
    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="{$func_name}">
                            $depth1
                        </select>
                    </dd>
                    <dd class="br">
                        {$posi_count}
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
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
                    <th>{$aft_ko}</th>
                    <td>
                        <div></div>
                        <div class="toggle_wrapper">
                            <div class="after_price" id="{$prefix}_price_dd"></div>
                            <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>

                            <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAoAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                        </div>

                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <select class="basic_input" id="{$prefix}_val" name="{$prefix}_val" onchange="{$func_name}">
                            {$depth1}
                        </select>
                    </td>
                </tr>
                {$posi_count}
            </table>
html;
    }

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 족자 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoScrollHtml($info, $dvs) {
    $name_en = "scroll";
    $depth1 = '';

    foreach ($info as $i) {
        $depth1 .= option($i["depth1"], $i["mpcode"]);
    }

    $html = <<<html
        <div class="option _{$name_en}">
            <dl>
                <dt>족자</dt>
                <dd class="price" id="{$dvs}_{$name_en}_price_dd"></dd>
                <dd>
                    <select id="{$dvs}_{$name_en}_val" name="{$dvs}_{$name_en}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                        {$depth1}
                    </select>
                </dd>
                <dd class="br">
                    <label><input name="{$dvs}_{$name_en}_dvs" value="상" type="checkbox" checked="checked" onclick="$(this).prop('checked', true);" /> 상</label>
                    <label><input name="{$dvs}_{$name_en}_dvs" value="하" type="checkbox" checked="checked" onclick="$(this).prop('checked', true);" /> 하</label>
                    <input type="hidden" id="{$dvs}_{$name_en}_info" name="{$dvs}_{$name_en}_info" value="상하" />
                    <input type="hidden" id="{$dvs}_{$name_en}_price" name="{$dvs}_{$name_en}_price" value="" />
                </dd>
            </dl>
        </div>
html;

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 쿨코팅 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoCoolCoatingHtml($info, $dvs) {
    $name_en = "cool_coating";
    $depth1 = '';

    foreach ($info as $i) {
        $depth1 .= option($i["depth1"], $i["mpcode"]);
    }

    $html = <<<html
        <div class="option _{$name_en}">
            <dl>
                <dt>쿨코팅</dt>
                <dd class="price" id="{$dvs}_{$name_en}_price_dd"></dd>
                <dd>
                    <select id="{$dvs}_{$name_en}_val" name="{$dvs}_{$name_en}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                        {$depth1}
                    </select>
                    <input type="hidden" id="{$dvs}_{$name_en}_info" name="{$dvs}_{$name_en}_info" value="" />
                    <input type="hidden" id="{$dvs}_{$name_en}_price" name="{$dvs}_{$name_en}_price" value="" />
                </dd>
            </dl>
        </div>
html;

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 타공 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoPunchingHtml($info, $dvs) {
    $name_en = "punching";
    $depth1 = '';

    foreach ($info as $i) {
        $depth1 .= option($i["depth1"], $i["mpcode"]);
    }

    $html = <<<html
        <div class="option _{$name_en}">
            <dl>
                <dt>미싱</dt>
                <dd class="price" id="{$dvs}_{$name_en}_price_dd"></dd>
                <dd>
                    <select id="{$dvs}_{$name_en}_val" name="{$dvs}_{$name_en}_val" onchange="getAoAfterPrice.common('{$name_en}', '{$dvs}');">
                        {$depth1}
                    </select>
                    <input type="hidden" id="{$dvs}_{$name_en}_info" name="{$dvs}_{$name_en}_info" value="" />
                    <input type="hidden" id="{$dvs}_{$name_en}_price" name="{$dvs}_{$name_en}_price" value="" />
                </dd>
            </dl>
        </div>
html;

    return $html;
}

/**
 * @brief 실사출력 추가 후공정 미싱 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getAoCuttingHtml($info, $dvs, $mobile_yn) {
    $aft_en = "cutting";
    $aft_ko = "재단";
    $prefix = $dvs . '_' . $aft_en;

    $depth1 = '';

    foreach ($info as $i) {
        $depth1 .= option($i["depth1"], $i["mpcode"]);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAoAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $depth1
                        </select>
                    </dd>
                    <dd class="br">
                        <label class="check"><input name="{$prefix}_dvs" value="상" type="checkbox" onchange="getAoAfterPrice.common('{$aft_en}', '{$dvs}');"><i><i></i></i> 상</label>
                        <label class="check"><input name="{$prefix}_dvs" value="하" type="checkbox" onchange="getAoAfterPrice.common('{$aft_en}', '{$dvs}');"><i><i></i></i> 하</label>
                        <label class="check"><input name="{$prefix}_dvs" value="좌" type="checkbox" onchange="getAoAfterPrice.common('{$aft_en}', '{$dvs}');"><i><i></i></i> 좌</label>
                        <label class="check"><input name="{$prefix}_dvs" value="우" type="checkbox" onchange="getAoAfterPrice.common('{$aft_en}', '{$dvs}');"><i><i></i></i> 우</label>
                        <input type="hidden" id="{$prefix}_info" name="{$prefix}_info" value="" />
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 후지반칼 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getHalfKnifeInfoHtml($info, $dvs, $mobile_yn, $def) {
    $util = new FrontCommonUtil();

    $aft_en = "halfknife";
    $aft_ko = "후지반칼";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $option = "";
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];

        $mpcode = $temp["mpcode"];
        $aft = $temp["depth1"];

        $class = "";

        $selected = "";
        if ($def["mpcode"] === $mpcode) {
            $selected = "selected=\"selected\"";
        }

        $option .= option($aft, $mpcode, $selected . ' ' .  $class);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('halfknife', '{$dvs}');">
                            $option
                        </select>
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                    <dd class="br note">
                                ※ 아트지 90g 스티커 반칼5cm간격 <br>
                                ※ 유포지 80g 반칼5cm간격<br>
                                ※ 모조지 80g 반칼5cm간격<br>
                                ※ 투명데드롱 38µ 후지반칼불가<br>
                                ※ 은데드롱 25g 후지반칼불가<br>
                                ※ 크라프트지 57g 후지반칼불가<br>
                    </dd>
                </dl>
                
            </div>
html;
    } else {
        $html = <<<html
                    <table class="basic_table after_{$aft_en}">
                        <colgroup>
                            <col style="width:20%;">
                            <col style="">
                        </colgroup>
                        <tr>
                            <th>{$aft_ko}</th>
                            <td>
                                <div></div>
                                <div class="toggle_wrapper">
                                    <div id="{$prefix}_price_dd" class="after_price"></div>
                                    <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                    <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('halfknife', '{$dvs}');" class="basic_input">
                                    $option
                                </select>
                                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            </td>
                        </tr>
                    </table>
html;
    }

    return $html;
}

/**
 * @brief 추가 후공정 전체빼다 정보 html 반환
 *
 * @param $info = 후공정 정보
 * @param $dvs  = 제품구분값
 *
 * @return div html
 */
function getBackgroundInfoHtml($info, $dvs, $mobile_yn, $def) {
    $util = new FrontCommonUtil();

    $aft_en = "background";
    $aft_ko = "전체빼다";
    $prefix = $dvs . '_' . $aft_en;

    $info_count = count($info);

    $option = "";
    for ($i = 0; $i < $info_count; $i++) {
        $temp = $info[$i];

        $mpcode = $temp["mpcode"];
        $aft = $temp["depth1"];

        $selected = "";
        if ($def["mpcode"] === $mpcode) {
            $selected = "selected=\"selected\"";
        }

        $option .= option($aft, $mpcode, $selected);
    }

    if (!$mobile_yn) {
        $html = <<<html
            <div class="option _{$aft_en}">
                <dl>
                    <dt>{$aft_ko}</dt>
                    <dd class="price" id="{$prefix}_price_dd"></dd>
                    <dd>
                        <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');">
                            $option
                        </select>
                        <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                    </dd>
                </dl>
            </div>
html;
    } else {
        $html = <<<html
                    <table class="basic_table after_{$aft_en}">
                        <colgroup>
                            <col style="width:20%;">
                            <col style="">
                        </colgroup>
                        <tr>
                            <th>{$aft_ko}</th>
                            <td>
                                <div></div>
                                <div class="toggle_wrapper">
                                    <div id="{$prefix}_price_dd" class="after_price"></div>
                                    <div class="toggle" action="aft" aft="{$aft_en}" dvs="{$dvs}"></div>
                                    <input type="checkbox" name="{$dvs}_chk_after[]" id="${prefix}" onclick="loadAfterPrice.exec(this.checked, this.value, '{$dvs}');" value="{$aft_ko}" aft="{$aft_en}" class="aft_chkbox" style="display:none;" />
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <select id="{$prefix}_val" name="{$prefix}_val" onchange="getAfterPrice.common('{$aft_en}', '{$dvs}');" class="basic_input">
                                    $option
                                </select>
                                <input type="hidden" id="{$prefix}_price" name="{$prefix}_price" value="" />
                            </td>
                        </tr>
                    </table>
html;
    }

    return $html;
}
?>
