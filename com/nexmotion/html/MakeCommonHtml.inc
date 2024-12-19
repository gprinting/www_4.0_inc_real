<?
/**
 * @brief option html 공통사용함수
 *
 * @param $val  = option 실제 값
 * @param $dvs  = option 화면 출력값
 * @param $attr = option에 추가로 입력할 attribute
 *
 * @return option html
 */
function option($dvs, $val = '' , $attr = '') {
    $option_form = "<option %s value=\"%s\">%s</option>";

    $ret = sprintf($option_form, $attr, $val, $dvs);

    return $ret;
}

function single_option($dvs, $i, $val = '' , $attr = '') {
    $option_form = "<label style='width: 100%%'><input %s type='radio' name='printType%s' value=\"%s\" onchange='changeData();'><span id='printType%s_%s'>%s</span></label>";

    $ret = sprintf($option_form, $attr, $i,$val, $i, $val, $dvs);

    return $ret;
}

function single_color_option($dvs, $i, $val = '' , $attr = '') {
    $option_form = "<label><input type='radio' name='color' value='%s' onchange='changeData();'><span class='bright' style='background-color: %s;'><i>%s</i></span></label>";

    $ret = sprintf($option_form, $val, $attr, $dvs);

    return $ret;
}
?>
