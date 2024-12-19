<?
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . '/define/front/product_info_class.inc');
include_once(INC_PATH . '/com/nexmotion/doc/front/product/AddOptInfo.inc');
include_once(INC_PATH . '/com/nexmotion/doc/front/product/AddAfterInfo.inc');
include_once(INC_PATH . '/com/nexmotion/doc/front/product/PriceInfo.inc');
include_once(INC_PATH . '/com/nexmotion/common/util/front/FrontCommonUtil.inc');

/**
 * @brief 카테고리 종이분류 option html 생성
 *
 * @param $rs      = 검색결과
 * @param $default = default selected 값
 * @param $price_info_arr = 가격검색용 정보저장 배열
 *
 * @return option html
 */
function makeCatePaperSortOption($rs, $default, &$price_info_arr) {
    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();

    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $sort   = $fields["sort"];

        $selected = "";
        if ($default === true || $sort === $default) {
            $selected = "selected=\"selected\"";

            $price_info_arr["paper_sort"] = $sort;

            $default = false;
        }

        $ret .= option($sort, $sort, $selected);

        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 종이명 option html 생성
 *
 * @param $rs      = 검색결과
 * @param $default = default selected 값
 * @param $price_info_arr = 가격검색용 정보저장 배열
 *
 * @return option html
 */
function makeCatePaperNameOption($rs, $default, &$price_info_arr) {

    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();
    

    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $name   = $fields["name"];   
        

        if($fields["cate_sortcode"] == "001002001" && ($name == "랑데뷰" || $name == "모조지" ))
        {
            $rs->MoveNext();
            continue;
        }

  
        $selected = "";
        if ($default === true || $name === $default) {
            
            $selected = "selected=\"selected\"";

            $price_info_arr["paper_name"] = $name;

            $default = false;
        }

        $ret .= option($name, $name, $selected);

    
    
        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 종이정보 option html 생성
 *
 * @param $rs      = 검색결과
 * @param $default = default selected 값
 * @param $price_info_arr = 가격검색용 정보저장 배열
 *
 * @return option html
 */
function makeCatePaperOption($rs, $default, $flag_arr, &$price_info_arr) {
    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();

    $sort_dup_chk = [];

    while($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $mpcode = $fields["mpcode"];
        $sort   = $fields["sort"];

        $info = $util->makePaperInfoStr($fields, $flag_arr);

        $selected = "";
        if ($default === true || $info === $default) {
            $selected = "selected=\"selected\"";
            if (!isset($price_info_arr["paper_name"])) {
                $price_info_arr["paper_name"]  = $fields["name"];
            }
            if (!isset($price_info_arr["paper_dvs"])) {
                $price_info_arr["paper_dvs"]   = $fields["dvs"];
            }
            if (!isset($price_info_arr["paper_color"])) {
                $price_info_arr["paper_color"] = $fields["color"];
            }

            $price_info_arr["paper_mpcode"] = $mpcode;

            $sort_dup_chk[$sort] = option($sort, $sort, $selected);

            $default = false;
        }

        if ($sort_dup_chk[$sort] === null) {
            $sort_dup_chk[$sort] = option($sort, $sort, $selected);
        }

        $ret .= option($info, $mpcode, $selected);

        $rs->MoveNext();
    }

    foreach ($sort_dup_chk as $val) {
        $ret_sort .= $val;
    }

    return array("info" => $ret,
                 "sort" => $ret_sort);
}


function makeCatePaperGroupOption($rs, $default, $flag_arr, &$price_info_arr, $cs) {
    $ret = "";
    $ret_sort = "";
    $enable_express = "";
    $disable_express = "";

    $util = new FrontCommonUtil();

    $sort_dup_chk = [];

    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $mpcode = $fields["mpcode"];
        $sort   = $fields["sort"];

        $info = $util->makePaperInfoStr($fields, $flag_arr);

        $selected = "";

        if ($default === true || $info === $default) {
            $selected = "selected=\"selected\"";
            if (!isset($price_info_arr["paper_name"])) {
                $price_info_arr["paper_name"]  = $fields["name"];
            }
            if (!isset($price_info_arr["paper_dvs"])) {
                $price_info_arr["paper_dvs"]   = $fields["dvs"];
            }
            if (!isset($price_info_arr["paper_color"])) {
                $price_info_arr["paper_color"] = $fields["color"];
            }

            $price_info_arr["paper_mpcode"] = $mpcode;

            $sort_dup_chk[$sort] = option($sort, $sort, $selected);

            $default = false;
        }

        if ($sort_dup_chk[$sort] === null) {
            $sort_dup_chk[$sort] = option($sort, $sort, $selected);
        }

        if($fields['name'] == "휘라레"
            || $fields['name'] == "반누보"
            || $fields['name'] == "스타드림"
            || $fields['name'] == "스타골드"
            || $fields['name'] == "스타실버"
            || $fields['name'] == "그레이스"
            || $fields['name'] == "뉴크라프트보드"
            || $fields['name'] == "랑데뷰"
            || $fields['name'] == "베이직"
            || $fields['name'] == "퓨리"
            || $fields['name'] == "매쉬멜로우"
            || $fields['name'] == "키칼라메탈릭"
            || $fields['name'] == "팝셋"
            || $fields['name'] == "컨셉블루펄"
            || $fields['name'] == "탄트지"
            || $fields['name'] == "빌리지") {
            $enable_express .= option($info, $mpcode, $selected);
        } else if($cs != "express"){
            $disable_express .= option($info, $mpcode, $selected);
        }

        $rs->MoveNext();
    }

    foreach ($sort_dup_chk as $val) {
        $ret_sort .= $val;
    }

    $enable_express = "<optgroup label='당일판 가능'>" . $enable_express . "</optgroup>";

    if($cs != "express") {
        $disable_express = "<optgroup label='당일판 불가'>" . $disable_express . "</optgroup>";
    }

    return array("info" => $enable_express . $disable_express,
        "sort" => $ret_sort);
}









function makeCateSinglePaperOption($rs, $default, $flag_arr, &$price_info_arr) {
    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();
    $i = 0;
    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $mpcode = $fields["mpcode"];
        $sort   = $fields["sort"];

        $info = $util->makeSingleInfoStr($fields, $flag_arr);
        $ret .= single_option($info, 1, $mpcode, $i == 0 ? "checked" : "");
        //$ret .= single_color_option($info, 1, $mpcode, $i == 0 ? "checked" : "");

        $i++;
        $rs->MoveNext();
    }

    return array("info" => $ret,
        "sort" => $ret_sort);
}

function makeCateSingleSizeOption($rs, $default, $flag_arr, &$price_info_arr) {
    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();
    $i = 0;
    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $mpcode = $fields["mpcode"];
        $sort   = $fields["sort"];

        $info = $util->makeSingleInfoStr($fields, $flag_arr);

        $ret .= single_option($info, 3, $mpcode, $i == 0? "checked" : "");

        $i++;
        $rs->MoveNext();
    }

    return $ret;
}

function makeCateSinglePrintOption($rs, $default, $flag_arr, &$price_info_arr) {
    $ret = "";
    $ret_sort = "";

    $util = new FrontCommonUtil();
    $i = 0;
    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        $input["name"] = explode('/',$fields["name"])[1];
        $input["dvs"] = explode('/',$fields["name"])[0];
        $mpcode = $fields["mpcode"];
        //$sort   = $fields["sort"];

        //$info = $util->makeSingleInfoStr($input, $flag_arr);
        if($input["name"] != null) {
            $ret .= single_color_option($input["name"], 2, $mpcode, $input["dvs"]);
        } else {
            $ret .= single_option($input["dvs"], 2, $mpcode, $i == 0? "checked" : "");
        }
        $i++;
        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 인쇄도수 option html 생성
 *
 * @param $rs             = 검색결과
 * @param $default_print  = default selected 값
 * @param $default_purp   = $price_info_arr에 저장할 맵핑코드 검색용
 * @param $price_info_arr = 가격검색용 정보저장 배열
 * @param $tmpt_dvs       = 도수구분(단/양면 => true, 전/후면 => false)
 *
 * @return 면 구분별 html 배열
 */
function makeCatePrintOption($rs,
                             $default_print,
                             $default_purp,
                             &$price_info_arr) {
    $ret = [];
    $dup_chk = [];

    // ProductDefaultSel 클래스에 기본 설정값 없는경우 바이패스값 설정
    if (empty($default_print)) {
        $default_print = true;
        $default_val = true;
    }
    if (empty($default_purp)) {
        $default_purp = true;
    }

    $purp_arr = [];
    $purp_html = '';

    while($rs && !$rs->EOF) {
        $mpcode   = $rs->fields["mpcode"];
        $name     = $rs->fields["name"];
        $side_dvs = $rs->fields["side_dvs"];
        $purp_dvs = $rs->fields["purp_dvs"];

        // ProductDefaultSel 클래스에 기본 설정값 있는경우
        if ($default_print !== true &&
                $side_dvs === "단면" || $side_dvs === "양면") {
            $default_val = $default_print;
        } else if ($default_print !== true &&
                $side_dvs === "전면") {
            $default_val = $default_print["bef_print"];
        } else if ($default_print !== true &&
                $side_dvs === "전면추가") {
            $default_val = $default_print["bef_add_print"];
        } else if ($default_print !== true &&
                $side_dvs === "후면") {
            $default_val = $default_print["aft_print"];
        } else if ($default_print !== true &&
                $side_dvs === "후면추가") {
            $default_val = $default_print["aft_add_print"];
        }

        $selected = '';
        if ($side_dvs === "단면" || $side_dvs === "양면") {
            if ($name === $default_val) {
                $selected = "selected=\"selected\"";

                $price_info_arr["print_mpcode"] = $mpcode;
                $price_info_arr["print_name"]   = $name;

                // 기본값 검색 후 조건문 다시 못들어오게 변수값 수정
                $default_print = false;
                $default_purp = false;
                $default_val = false;
            }
            /*
            // 기본 설정값 없거나
            if ((is_bool($default_val) && $default_val) ||
                    (is_bool($default_purp) && $default_purp)) {
                $selected = "selected=\"selected\"";

                $price_info_arr["print_mpcode"] = $mpcode;
                $price_info_arr["print_name"]   = $name;

                // 기본값 검색 후 조건문 다시 못들어오게 변수값 수정
                $default_print = false;
                $default_purp = false;
                $default_val = false;
            }
            // 기본 설정값이랑 검색값이랑 같은경우
            if ($name === $default_val && $purp_dvs === $default_purp) {
                $selected = "selected=\"selected\"";

                $price_info_arr["print_mpcode"] = $mpcode;
                $price_info_arr["print_name"]   = $name;

                // 기본값 검색 후 조건문 다시 못들어오게 변수값 수정
                $default_print = false;
                $default_purp = false;
                $default_val = false;
            }
            // 주문페이지에서 인쇄방식 변경한경우
            if ($name === $default_val && is_int($default_purp)) {
                $selected = "selected=\"selected\"";

                $price_info_arr["print_mpcode"] = $mpcode;
                $price_info_arr["print_name"]   = $name;

                // 기본값 검색 후 조건문 다시 못들어오게 변수값 수정
                $default_print = false;
                $default_purp = false;
                $default_val = false;
            }
            */
        } else {
            if ((is_bool($default_val) && $default_val) ||
                    (is_bool($default_purp) && $default_purp)) {
                $temp = $price_info_arr["print_mpcode"][$side_dvs]["mpcode"];
                if (empty($temp)) {
                    $selected = "selected=\"selected\"";

                    $price_info_arr["print_mpcode"][$side_dvs] = array(
                        "mpcode" => $mpcode,
                        "name"   => $name
                    );
                }
            }
            if ($name === $default_val && $purp_dvs === $default_purp) {
                $temp = $price_info_arr["print_mpcode"][$side_dvs]["mpcode"];
                if (empty($temp)) {
                    $selected = "selected=\"selected\"";

                    $price_info_arr["print_mpcode"][$side_dvs] = array(
                        "mpcode" => $mpcode,
                        "name"   => $name
                    );
                }
            }
            if ($name === $default_val && is_int($default_purp)) {
                $temp = $price_info_arr["print_mpcode"][$side_dvs]["mpcode"];
                if (empty($temp)) {
                    $selected = "selected=\"selected\"";

                    $price_info_arr["print_mpcode"][$side_dvs] = array(
                        "mpcode" => $mpcode,
                        "name"   => $name
                    );
                }
            }
        }

        if (empty($purp_arr[$purp_dvs])) {
            $purp_arr[$purp_dvs] = true;

            $purp_sel = '';
            if ($purp_dvs === $default_purp) {
                $purp_sel = "selected=\"selected\"";
            }

            $purp_html .= option($purp_dvs, $purp_dvs, $purp_sel);
        }

        /*
        // 기본선택된 인쇄방식 있을경우 그것만 html 생성
        if (!is_int($default_purp) && 
                !is_bool($default_purp) &&
                $purp_dvs !== $default_purp) {
            $rs->MoveNext();
            continue;
        }
*/
        // 중복 option 제거
        $key = $name . '!' . $side_dvs;
        if ($dup_chk[$key] === null) {
            $dup_chk[$key] = true;
        } else {
            $rs->MoveNext();
            continue;
        }

        if ($ret[$side_dvs] === null) {
            $ret[$side_dvs] = option($name, $mpcode, $selected);
        } else {
            $ret[$side_dvs] .= option($name, $mpcode, $selected);
        }

        $rs->MoveNext();
    }

    $ret["purp_dvs"] = $purp_html;

    return $ret;
}

/**
 * @brief 카테고리 사이즈 option html 생성
 *
 * @param $rs             = 검색결과
 * @param $default        = default selected 값
 * @param $pos_num_arr    = 사이즈별 자리수 배열
 * @param $price_info_arr = 가격검색용 정보저장 배열
 * @param $affil_yn       = 계열 노출여부
 * @param $size_typ_yn    = 사이즈 타입명 노출여부
 *
 * @return option html
 */
function makeCateSizeOption($rs,
                            $default,
                            $pos_num_arr,
                            &$price_info_arr,
                            $affil_yn,
                            $size_typ_yn) {
    $ret = "";

    $attr_form  = "class=\"";
    $attr_form .= "_workingWH%s-%s ";
    $attr_form .= "_cuttingWH%s-%s ";
    $attr_form .= "_thomsonWH%s-%s ";
    $attr_form .= "_designWH%s-%s";
    $attr_form .= "\" ";
    $attr_form .= "pos_num=\"%s\" ";
    $attr_form .= "affil=\"%s\"";

    $pos_num = 0;

    while($rs && !$rs->EOF) {
        $mpcode = $rs->fields["mpcode"];
        $name   = $rs->fields["name"];
        $typ    = $rs->fields["typ"];
        $affil  = $rs->fields["affil"];

        if (!$affil_yn) {
            $affil = '';
        }

        $work_wid_size  = $rs->fields["work_wid_size"];
        $work_vert_size = $rs->fields["work_vert_size"];
        $cut_wid_size  = $rs->fields["cut_wid_size"];
        $cut_vert_size = $rs->fields["cut_vert_size"];
        $tomson_wid_size  = $rs->fields["tomson_wid_size"];
        $tomson_vert_size = $rs->fields["tomson_vert_size"];
        $design_wid_size  = $rs->fields["design_wid_size"];
        $design_vert_size = $rs->fields["design_vert_size"];

        if (is_array($pos_num_arr) === true) {
            $pos_num = $pos_num_arr[$name];
        }

        if ($size_typ_yn) {
            $name .= sprintf("(%s)", $typ);
        }

        $attr = sprintf($attr_form, $work_wid_size
                                  , $work_vert_size
                                  , $cut_wid_size
                                  , $cut_vert_size
                                  , $tomson_wid_size
                                  , $tomson_vert_size
                                  , $design_wid_size
                                  , $design_vert_size
                                  , $pos_num
                                  , $affil);

        if($default == "비규격") $default = true;

        $selected = "";
        if ($default === true || $name === $default) {

            $selected = " selected=\"selected\"";

            $price_info_arr["size_name"]    = $rs->fields["name"];
            $price_info_arr["affil"]        = $affil;
            $price_info_arr["pos_num"]      = $pos_num;
            $price_info_arr["stan_mpcode"]  = $mpcode;
            $price_info_arr["def_cut_wid"]  = $cut_wid_size;
            $price_info_arr["def_cut_vert"] = $cut_vert_size;
            $price_info_arr["size_gap"]     = intval($work_wid_size) -
                                              intval($cut_wid_size);

            $default = false;

        }

        $attr .= $selected;

        $ret .= option($name, $mpcode, $attr);

        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 수량 option html 생성
 *
 * @param $rs       = 검색결과
 * @param $amt_unit = 수량단위
 * @param $default  = default selected 값
 * @param $price_info_arr = 가격검색용 정보저장 배열
 *
 * @return option html
 */
function makeCateAmtOption($rs, $amt_unit, $default, &$price_info_arr, $divide = 1) {
    $ret = '';
    $default = doubleval($default);
    if($divide < 1) $divide = 1;

    while($rs && !$rs->EOF) {
        $amt = $rs->fields["amt"] * $divide;
        $amt = doubleval($amt);

        $selected = "";
        if ($default === 0.0 || $amt === $default) {
            $selected = " selected=\"selected\"";

            $price_info_arr["amt"] = $amt;

            $default = -1;
        }

        $ciphers = 0;
        if ($amt < 1) {
            $ciphers = 1;
        }

        $amt_format = number_format($amt, $ciphers);
        $amt_format .= ' ' . $amt_unit;

        $ret .= option($amt_format, $amt, $selected);

        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 수량 option html 생성
 *
 * @param $rs  = 검색결과
 *
 * @return option html
 */
function makeCatePackWayOption($rs) {
    $ret = '';

    while($rs && !$rs->EOF) {
        $name = $rs->fields["pack_name"];

        $ret .= option($name, $name);

        $rs->MoveNext();
    }

    return $ret;
}

/**
 * @brief 카테고리 옵션 ul html 생성
 *
 * @param $rs = 검색결과
 * @param &$info_arr = 추가 옵션 팝업 생성용 정보배열
 *
 * @return ul html
 */
function makeCateOptUl($rs, &$info_arr, $def_arr , $cate_sortcode, $link) {
    if ($rs->EOF) {
        return "<ul>없음</ul>";
    }

    $dvs = $info_arr["dvs"];

    $is_ao = $info_arr["is_ao"];
    $func_name = "loadOptPrice.exec";
    if ($is_ao) {
        $func_name = "loadAoOptPrice.exec";
    }

    $ret = "<ul style='display: flex; justify-content: space-between; width:100%'>";

    $li_form = "<li><label class=\"check\"><input type=\"checkbox\" name=\"chk_opt[]\" id=\"opt_%s\" value=\"%s\" price=\"\" onclick=\"%s(this, '%s', '%s');\" %s /><i><i></i></i> %s</label></li>";

    // 추가 옵션
    $add = "";
    $i = 0;
    $time = intval(date("Gis"));
    $dup_chk = [];
    while($rs && !$rs->EOF) {
        $mpcode   = $rs->fields["mpcode"];
        $name     = $rs->fields["name"];
        $depth1   = $rs->fields["depth1"];
        $basic_yn = $rs->fields["basic_yn"];
        $sortcode = $rs->fields["cate_sortcode"]; // 180413 합판전단때문에 추가

        $def = $def_arr["name"];

        $checked = '';
        // 당일판 옵션 시간에 따라 처리
        if ($name === "당일판") {
            // 개발완료시 주석해제
            // 3시 넘으면 안보이게 처리 

            if( 150000 <= $time && $time > 0 ){
                $rs->MoveNext();
                continue;
            }
            
   /*         
            if (120000 <= $time && $time < 190000) {

               // echo $time;
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후12시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }

            if (150000 <= $time && $time < 190000) {
                // 12시 이전이면 15시 pass
                echo $time;
                if ($depth1 === "오후3시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            } 
*/

            /*
            else if (133000 <= $time && $time < 190000) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후1시30분 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            else if (123000 <= $time && $time < 190000) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후2시30분 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            else if (140000 <= $time && $time < 190000) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후2시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }

            */

            /*
            if ($time < 150000) {
                $checked = "checked=\"checked\"";
                //if ($sortcode === "005001001" || $sortcode === "003002001") {
                    $checked = "";
                //}
            } else if (150000 <= $time && $time < 200000) {
                //$checked = "disabled=\"disabled\"";
                $rs->MoveNext();
                continue;
            }
            */
        }  else if (!empty($def)) {
            $checked = "checked=\"checked\"";
        } else {
            $key = $name . '!' . $basic_yn;

            if ($dup_chk[$key] === null) {
                $dup_chk[$key] = true;
            } else {
                $rs->MoveNext();
                continue;
            }

            if ($basic_yn === "Y") {
                $rs->MoveNext();
                continue;
            }
        }

        $add .= sprintf($li_form, $i
                                , $name
                                , $func_name
                                , $i
                                , $dvs
                                , $checked
                                , $name);

        $info_arr["idx"][$name] = $i;
        $info_arr["name"][$i++] = $name;

        $rs->MoveNext();
    }

    $ret .= $add;
    if($add == "")
    {
        $sizeLi = "325px";
    }
        else
    {
        $sizeLi = "220px";
    }
    if($cate_sortcode == "'003002001'" && $link == "") {$ret .= "<li style='margin-left:$sizeLi'>*조판상황에 따라 디지털로 진행 될 수 있습니다</li>";}
    $ret .= "</ul>";

    return $ret;
}

/**
 * @brief 카테고리 후공정 ul html 생성
 *
 * @param $rs  = 검색결과
 * @param $dvs = 제품 구분값
 * @param &$info_arr  = 추가 후공정 팝업 생성용 정보배열
 * @param $except_arr = html을 생성하지 않을 후공정명 배열
 *
 * @return ul html
 */
function makeCateAftUl($rs, $dvs, &$info_arr, $def_arr, $except_arr = []) {
    if ($rs->EOF) {
        return "<ul>없음</ul>";
    }

    $li_class_arr = ProductInfoClass::AFTER_ARR;

    $ret = "<ul>";

    $li_form = "<li %s><label class=\"check\"><input type=\"checkbox\" name=\"%s_chk_after[]\" id=\"%s_%s\" value=\"%s\" aft=\"%s\" class=\"aft_chkbox\" %s /><i><i></i></i> %s</label></li>";

    // 추가 후공정
    $add = "";

    $i = 0;
    $j = 0;
    $dup_chk = [];
    while($rs && !$rs->EOF) {
        $name     = $rs->fields["name"];
        $name1     = $rs->fields["name"];

     /*   if($name1 == "엠보싱"){
            $name1 = "에폭시";
        } */
        
        $basic_yn = $rs->fields["basic_yn"];
        if($dvs == "ev" && $name == "접착") $name1 = "양면테이프";

        if ($except_arr[$name] !== null) {
            $rs->MoveNext();
            continue;
        }

        $key = $name . '!' . $basic_yn;

        if ($dup_chk[$key] === null) {
            $dup_chk[$key] = true;
        } else {
            $rs->MoveNext();
            continue;
        }

        $class = $li_class_arr[$name];
        $li_class = ($class === null) ? '' : "class=\"_" . $class . "\"";

        if ($basic_yn === "Y") {
            $info_arr["basic"][$j++] = $name;
            $rs->MoveNext();
            continue;
        }

        $checked =  '';
        $def = $def_arr[$name];
        if (!empty($def)) {
            $checked = "checked=\"checked\"";
        }

        // 상민 : 20240320 사방쇠고리 작업
        if($name1==="고리"){
            $checked = "checked";
        }

        $add .= sprintf($li_form, $li_class
                                , $dvs
                                , $dvs
                                , $class
                                , $name
                                , $class
                                , $checked
                                , $name1);

        $info_arr["add"][$i++] = $name;

        $rs->MoveNext();
    }

    $ret .= $add;
    $ret .= "</ul>";

     // 상민 : 20240320 사방쇠고리 작업
    $ret .= <<<'AUTOFOLD'
<script>
    setTimeout(function(){
        $('.overview.ao_overview > ._folding').click();
    }, 300);
</script>
AUTOFOLD;

    return $ret;
}

/**
 * @brief 카테고리 옵션 ul html 생성
 *
 * @param $rs      = 검색결과
 * @param $idx_arr = 옵션 위치 인덱스 배열
 * @param $dvs     = 제품 구분값
 *
 * @return ul html
 */
function makeCateAddOpt($rs, $idx_arr, $dvs, $mobile_yn, $is_ao, $def_arr) {
    $ret = "";

    $info_arr = [];

    $i = 0;
    $time = intval(date("Gis"));
    while($rs && !$rs->EOF) {
        $mpcode = $rs->fields["mpcode"];

        $name   = $rs->fields["opt_name"];
        $depth1 = $rs->fields["depth1"];
        $depth2 = $rs->fields["depth2"];
        $depth3 = $rs->fields["depth3"];

        if ($info_arr[$name] === null) {
            $i = 0;
        }

        // 당일판 옵션 시간에 따라 처리
        $date = strtotime(date() . '+' . $z++ . ' day');
        if(date('w', $date) != 0
            && date('w', $date) != 6) {

         /*   if($time < 150000 && $time > 0){
                if (120000 <= $time && $time > 0) {

                     if ($depth1 === "오후12시 마감") {
                        $rs->MoveNext();
                        continue;
                    }
                
                }else{
                    
                    if ($depth1 === "오후3시 마감") {
                        $rs->MoveNext();
                        continue;
                    }
                }
            } */
/*
            if ($time < 150000 && $time < 0) {
                // 12시 이전이면 15시 pass
                if ($depth1 === "오후3시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            } else if (120000 <= $time && $time > 0) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후12시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            else if (133000 <= $time && $time < 0) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후1시30분 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            else if (123000 <= $time && $time < 0) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후2시30분 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            else if (140000 <= $time && $time < 0) {
                // 12시 에서 15시 사이이면 12시 pass
                if ($depth1 === "오후2시 마감") {
                    $rs->MoveNext();
                    continue;
                }
            }
            */
        }

        if ($name === "빠른출고") {
            $z = 0;
            if($time >= 120000) $z++;
            while(1) {
                $date = strtotime(date() . '+' . $z++ . ' day');
                if(date('w', $date) != 0
                    && date('w', $date) != 6
                    && !in_array(date('Y-m-d', $date), ProductInfoClass::HOLODAYS)) {
                    break;
                }
            }

            if($depth1 === "내일출고") {
                while(1) {
                    $date = strtotime(date() . '+' . $z++ . ' day');
                    if(date('w', $date) != 0
                        && date('w', $date) != 6
                        && !in_array(date('Y-m-d', $date), ProductInfoClass::HOLODAYS)) {
                        break;
                    }
                }
            }
            /*
            if($time >= 120000){
                while(1) {
                    $date = strtotime(date() . '+' . $z++ . ' day');
                    if(date('w', $date) != 0
                        && date('w', $date) != 6
                        && !in_array(date('Y-m-d', $date), ProductInfoClass::HOLODAYS)) {
                        break;
                    }
                }
            }*/

            $info_arr[$name][$i]["out_date"]   = $date;
        }
        $info_arr[$name]["dvs"] = $dvs;
        $info_arr[$name][$i]["mpcode"]   = $mpcode;
        $info_arr[$name][$i]["depth1"]   = $depth1;
        $info_arr[$name][$i]["depth2"]   = $depth2;
        $info_arr[$name][$i++]["depth3"] = $depth3;

        $rs->MoveNext();
    }

    unset($rs);

    $ret = "";

    $info_arr_count = count($info_arr);
    $i = 0;
    foreach ($info_arr as $opt_name => $info) {
        $def = $def_arr[$opt_name];

        switch ($opt_name) {
            case "추가물품" :
                $ret .= getAddGoodsInfoHtml($idx_arr[$opt_name],
                                            $opt_name, $info, $mobile_yn, $def);
                break;
            case "거치대" :
                $ret .= getAddAoOptInfoHtml($idx_arr[$opt_name],
                    $opt_name, $info,
                    $mobile_yn, $is_ao, $def);
                break;
            case "빠른출고" :
                $ret .= getAddOptInfoHtml($idx_arr[$opt_name],
                    $opt_name, $info,
                    $mobile_yn, $is_ao, $def);
                break;
            default :
                $ret .= getAddOptInfoHtml($idx_arr[$opt_name],
                                          $opt_name, $info,
                                          $mobile_yn, $is_ao, $def);
                break;
        }
        if ($mobile_yn) {
            $ret .= "<hr class=\"after_section\">";
        }
    }

    return $ret;
}

/**
 * @brief 카테고리 후공정 ul html 생성
 *
 * @param $rs         = 검색결과
 * @param $dvs        = 제품 구분값
 * @param $except_arr = html 생성 제외 후공정
 *
 * @return ul html
 */
function makeCateAddAfter($rs, $dvs, $mobile_yn, $def_arr, $except_arr = [], $foil_count) {
    $ret = "";

    $info_arr = [];
    $key_form = "%s!%s!%s!%s";

    $i = 0;
    while($rs && !$rs->EOF) {
        $mpcode = $rs->fields["mpcode"];

        $name   = $rs->fields["after_name"];
        $depth1 = $rs->fields["depth1"];
        $depth2 = $rs->fields["depth2"];
        $depth3 = $rs->fields["depth3"];

        $key = sprintf($key_form, $name, $depth1, $depth2, $depth3);

        if ($except_arr[$name] !== null) {
            $rs->MoveNext();
            continue;
        }

        if ($info_arr[$name] === null) {
            $i = 0;
        }

        $info_arr[$name][$i]["mpcode"]   = $mpcode;
        $info_arr[$name][$i]["depth1"]   = $depth1;
        $info_arr[$name][$i]["depth2"]   = $depth2;
        $info_arr[$name][$i++]["depth3"] = $depth3;

        $rs->MoveNext();
    }

    unset($rs);

    $ret = "";

    foreach ($info_arr as $after_name => $info) {
        $def = $def_arr[$after_name];
        if($def) {
            $def = array();
            $def["mpcode"] = $def_arr[$after_name . "_mpcode"];
            $def["info"] = $def_arr[$after_name . "_info"];
            $def["depth1"] = $def_arr[$after_name . "_depth1"];
            $def["depth2"] = $def_arr[$after_name . "_depth2"];
            $def["depth3"] = $def_arr[$after_name . "_depth3"];
        }
        switch ($after_name) {
            case "코팅" :
                $ret .= getCoatingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "귀도리" :
                $ret .= getRoundingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "엣지박" :
                $ret .= getEdgeFoilInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "박" :
                $ret .= getFoilInfoHtml($info, $dvs, $mobile_yn, $def, $foil_count);
                break;
            case "형압" :
                $ret .= getPressInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "엠보싱" :
                $ret .= getEmbossingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "오시" :
            case "도무송오시" :
                $ret .= getImpressionInfoHtml($info, $dvs, $mobile_yn,
                                              $def, $after_name);
                break;
            case "미싱" :
                $ret .= getDotlineInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "타공" :
                $ret .= getPunchingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "접지" :
                $ret .= getFoldlineInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "도무송" :
                $ret .= getTomsonInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "넘버링" :
                $ret .= getNumberingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "재단" :
                $ret .= getCuttingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "제본" :
                $ret .= getBindingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "접착" :
                $ret .= getBondingInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "가공" :
                $ret .= getManufactureInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "복권실크" :
                $ret .= getLotterySilkInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "라미넥스" :
                $ret .= getLaminexInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "후지반칼" :
                $ret .= getHalfKnifeInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
            case "전체빼다" :
                $ret .= getBackgroundInfoHtml($info, $dvs, $mobile_yn, $def);
                break;
        }

        if ($mobile_yn) {
            $ret .= "<hr class=\"after_section\">";
        }
    }
    return $ret;
}

/**
 * @brief 카테고리 후공정 ul html 생성
 *
 * @param $rs       = 검색결과
 * @param $dvs      = 제품 구분값
 *
 * @return ul html
 */
function makeCateAoBasicAfter($rs, $dvs, $mobile_yn) {
    $ret = "";

    $info_arr = [];
    $key_form = "%s!%s!%s!%s";

    $i = 0;
    while($rs && !$rs->EOF) {
        $mpcode = $rs->fields["mpcode"];

        $name      = $rs->fields["after_name"];
        $depth1    = $rs->fields["depth1"];
        $depth2    = $rs->fields["depth2"];
        $depth3    = $rs->fields["depth3"];

        if ($info_arr[$name] === null) {
            $i = 0;
        }

        $info_arr[$name][$i]["mpcode"] = $mpcode;
        $info_arr[$name][$i]["depth1"] = $depth1;
        $info_arr[$name][$i]["depth2"] = $depth2;
        $info_arr[$name][$i++]["depth3"] = $depth3;

        $rs->MoveNext();
    }

    unset($rs);

    $ret = "";

    $after_arr = ProductInfoClass::AFTER_ARR;

    foreach ($info_arr as $after_name => $info) {
        $ret .= getAoBasicAfterHtml($dvs,
                                    $after_name,
                                    $after_arr[$after_name],
                                    $info,
                                    $mobile_yn);
    }

    return $ret;
}

/**
 * @brief 실사출력 카테고리 후공정 ul html 생성
 *
 * @param $rs         = 검색결과
 * @param $dvs        = 제품 구분값
 * @param $except_arr = html 생성 제외 후공정
 *
 * @return ul html
 */
function makeCateAddAoAfter($rs, $dvs, $mobile_yn, $except_arr = []) {
    $ret = "";

    $info_arr = [];
    $key_form = "%s!%s!%s!%s";

    $i = 0;
    while($rs && !$rs->EOF) {
        $mpcode = $rs->fields["mpcode"];

        $name   = $rs->fields["after_name"];
        $depth1 = $rs->fields["depth1"];
        $depth2 = $rs->fields["depth2"];
        $depth3 = $rs->fields["depth3"];

        $key = sprintf($key_form, $name, $depth1, $depth2, $depth3);

        if ($except_arr[$name] !== null) {
            $rs->MoveNext();
            continue;
        }

        if ($info_arr[$name] === null) {
            $i = 0;
        }

        $info_arr[$name][$i]["mpcode"]   = $mpcode;
        $info_arr[$name][$i]["depth1"]   = $depth1;
        $info_arr[$name][$i]["depth2"]   = $depth2;
        $info_arr[$name][$i++]["depth3"] = $depth3;

        $rs->MoveNext();
    }

    unset($rs);

    $ret = "";

    foreach ($info_arr as $after_name => $info) {
        switch ($after_name) {
            case "열재단" :
                $ret .= getAoCuttingHtml($info, $dvs, $mobile_yn);
                break;
            case "추가미싱" :
                $ret .= getAoDotlineHtml($info, $dvs, "add_dotline", $mobile_yn);
                break;
            case "미싱" :
                $ret .= getAoDotlineHtml($info, $dvs, "dotline", $mobile_yn);
                break;
            case "고리" :
                $ret .= getAoRingHtml($info, $dvs, $mobile_yn);
                break;
            case "족자" :
                $ret .= getAoScrollHtml($info, $dvs, $mobile_yn);
                break;
            case "쿨코팅" :
                $ret .= getAoCoolCoatingHtml($info, $dvs, $mobile_yn);
                break;
            case "타공" :
                $ret .= getAoPunchingHtml($info, $dvs, $mobile_yn);
                break;
            case "재단" :
                $ret .= getAoCuttingHtml($info, $dvs, $mobile_yn);
                break;
        }

        if ($mobile_yn) {
            $ret .= "<hr class=\"after_section\">";
        }
    }

    return $ret;
}

/**
 * @brief 인쇄유형 option html 반환
 *
 * @param $mono_dvs = 카테고리 계산방식
 *
 * @return option html
 */
function makeMonoDvsOption($mono_dvs) {
    $ret = "";

    if ($mono_dvs === '1') {
        $ret .= option("일반형(합판)", '0', "selected=\"selected\"");
        $ret .= option("계산형(독판)", '1');
    } else if ($mono_dvs === '2') {
        $ret .= option("일반형(합판)", '0');
    } else if ($mono_dvs === '3') {
        $ret .= option("계산형(독판)", '1');
    }

    return $ret;
}

/**
 * @brief 회원등급 할인 dl html 반환
 *
 * @param $param = 할인율, 회원등급 파라미터
 *
 * @return option html
 */
function makeGradeSaleDl($param) {
    return getGradeSaleDl($param);
}

/**
 * @brief 회원 수량별 할인 dl html 반환
 *
 * @param $param = 할인요율, 할인금액
 *
 * @return option html
 */
function makeAmtMemberSale($param) {
    return getAmtMemberSale($param);
}

/**
 * @brief 이벤트 할인 dl html 반환
 *
 * @param $param = 이벤트명, 할인 요율/가격 정보 파라미터
 *
 * @return option html
 */
function makeEventSaleDl($param) {
    return getEventSaleDl($param);
}

/**
 * @brief 책자형 제본 셀렉트박스 option html 생성
 *
 * @param $rs    = 제본 검색결과
 * @param $param = 기본값 등 파라미터
 * @param $price_info_arr = 가격검색용 정보저장 배열
 *
 * @return option html
 */
function makeBindingOptionHtml($rs, $param, &$price_info_arr) {
    $dvs     = $param["dvs"];
    $default = $param["default"];
    $html = '';

    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $aft    = $fields[$dvs];
        $mpcode = $fields["mpcode"];

        if (empty($mpcode)) {
            $mpcode = $aft;
        }

        $selected = '';
        if ($default === true || $aft === $default) {
            $price_info_arr["binding_" . $dvs] = $aft;
            $price_info_arr["binding_mpcode"]  = $mpcode;

            $selected = "selected=\"selected\"";

            $default = false;
        }

        $html .= option($aft, $mpcode, $selected);

        $rs->MoveNext();
    }

    return $html;
}
?>
