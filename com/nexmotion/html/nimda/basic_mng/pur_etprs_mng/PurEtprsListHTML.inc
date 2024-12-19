<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/doc/nimda/basic_mng/pur_etprs_mng/PurEtprsListDOC.inc");


/*
 * 매입업체 list 생성
 * 
 * return : list
 *
function makeExtnlEtprsList($result) {

    while($result && !$result->EOF) {

        $seqno = $result->fields["extnl_etprs_seqno"];
        $manu_name = $result->fields["manu_name"];
        $tel_num = $result->fields["tel_num"];
        $fax = $result->fields["fax"];
        $addr = $result->fields["addr"];
        $addr_detail = $result->fields["addr_detail"];

        $list  = "\n  <tr>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td><button class=\"orge btn_pu btn fix_height20 fix_width40\" onclick=\"editPopEtprs('%s')\">수정</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $manu_name, $tel_num, $fax,
                $addr . " " . $addr_detail,
                $seqno);

        $result->moveNext();
    }

    return $ret;
}*/

/*
 * 매입업체 list 생성
 * 
 * return : list
 */
function makeExtnlEtprsList($result) {

    while($result && !$result->EOF) {

        $seqno = $result->fields["extnl_etprs_seqno"];
        $etprs_name = $result->fields["etprs_name"];
        $licensee_num = $result->fields["crn"];
        $repre_name = $result->fields["repre_name"];
        $tob = $result->fields["tob"];
        $regi_date = $result->fields["regi_date"];

        $list  = "\n  <tr>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td><button class=\"orge btn_pu btn fix_height20 fix_width40\" onclick=\"editPopEtprs('%s')\">수정</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $etprs_name, $licensee_num, $repre_name,
                $tob, $regi_date,
                $seqno);

        $result->moveNext();
    }

    return $ret;
}

/*
 * 매입업체 종이 리스트
 * 
 * return : list
 */
function makePaperTbody($result) {
    
    $ret = "";
    $i = 1;

    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='14'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while ($result && !$result->EOF) {

        $param = array();
        $paper_brand = $result->fields["brand"];
        $paper_seqno = $result->fields["paper_seqno"];
        $paper_sort_name = $result->fields["sort"];
        $affil = $result->fields["affil"];
        $paper_name = $result->fields["name"];
        $dvs = $result->fields["dvs"];
        $color = $result->fields["color"];
        $basisweight = $result->fields["basisweight"];
        $wid_size = $result->fields["wid_size"];
        $vert_size = $result->fields["vert_size"];
        $crtr_unit = $result->fields["crtr_unit"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
		$list .="\n       <td><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width75\" onclick=\"supplyPopDetail(event, %d,'종이')\">보기</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $paper_brand, $paper_sort_name,
                        $affil, $paper_name,
                        $dvs, $color, $basisweight, $wid_size,
                        $vert_size, $crtr_unit, $paper_seqno);
        $result->moveNext();
    }

    return $ret; 
}

/*
 * 매입업체 출력 리스트
 * 
 * return : list
 */
function makeOutputTbody($result) {
    
    $ret = "";
    $i = 1;
    
    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='10'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while ($result && !$result->EOF) {

        $param = array();
        $output_brand = $result->fields["brand"];
        $output_seqno = $result->fields["output_seqno"];
        $output_board_name = $result->fields["board"];
        $output_top_name = $result->fields["top"];
        $affil = $result->fields["affil"];
        $output_name = $result->fields["name"];
        $wid_size = $result->fields["wid_size"];
        $vert_size = $result->fields["vert_size"];
        $crtr_unit = $result->fields["crtr_unit"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
		$list .="\n       <td><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width75\" onclick=\"supplyPopDetail(event, %d,'출력')\">보기</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $output_brand, $output_top_name, $affil,
                        $output_name, $output_board_name,
                        $wid_size, $vert_size, 
                        $crtr_unit, $output_seqno);
        $result->moveNext();
    }

    return $ret; 
}


/*
 * 매입업체 인쇄 리스트
 * 
 * return : list
 */
function makePrintTbody($result) {
    
    $ret = "";
    $i = 1;

    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='9'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while ($result && !$result->EOF) {

        $param = array();
        $print_brand = $result->fields["brand"];
        $print_seqno = $result->fields["print_seqno"];
        $print_tmpt_name = $result->fields["crtr_tmpt"];
        $print_top_name = $result->fields["top"];
        $affil = $result->fields["affil"];
        $output_name = $result->fields["name"];
        $wid_size = $result->fields["wid_size"];
        $vert_size = $result->fields["vert_size"];
        $crtr_unit = $result->fields["crtr_unit"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
		$list .="\n       <td><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width75\" onclick=\"supplyPopDetail(event, %d,'인쇄')\">보기</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $print_brand, $print_top_name,
                        $affil, $output_name, $print_tmpt_name,
                        $wid_size, $vert_size, 
                        $crtr_unit, $print_seqno);
        $result->moveNext();
    }

    return $ret; 
}

/*
 * 매입업체 후공정 리스트
 * 
 * return : list
 */
function makeAfterOptTbody($result) {
    
    $ret = "";
    $i = 1;

    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='7'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while ($result && !$result->EOF) {

        $param = array();
        $brand = $result->fields["brand"];
        $seqno = $result->fields["seqno"];
        $crtr_unit = $result->fields["crtr_unit"];
        $name = $result->fields["name"];
        $depth1 = $result->fields["depth1"];
        $depth2 = $result->fields["depth2"];
        $depth3 = $result->fields["depth3"];
        $pur_prdt = $result->fields["pur_prdt"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
		$list .="\n       <td><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width75\" onclick=\"supplyPopDetail(event, %d,'%s')\">보기</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $brand, $name, $depth1,
                        $depth2, $depth3, $crtr_unit, $seqno, $pur_prdt);
        $result->moveNext();
    }

    return $ret; 
}

/*
 * 매입업체 THEAD
 * 
 * return : list
 */
function makePaperThead($param) {

    $ret = "";

    $list .= "\n  <tr>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer;\" onclick=\"sortList('brand', this)\" class=\"blue_text01 sorting\">브랜드 <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">종이대분류</th>";
    $list .= "\n      <th class=\"bm2px\">계열</th>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer;\" onclick=\"sortList('name', this)\" class=\"blue_text01 sorting\">종이명  <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">구분</th>";
    $list .= "\n      <th class=\"bm2px\">색상</th>";
    $list .= "\n      <th class=\"bm2px\">평량</th>";
    $list .= "\n      <th class=\"bm2px\">가로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">세로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">기준단위</th>";
    $list .= "\n      <th class=\"bm2px\">조회</th>";
    $list .= "\n  </tr>";

    $ret .= sprintf($list, $param["brand_sort"], $param["brand_sort_type"],
                    $param["name_sort"], $param["name_sort_type"]);

    return $ret; 
}

/*
 * 매입업체 THEAD
 * 
 * return : list
 */
function makeOutputThead($param) {

    $ret = "";

    $list .= "\n  <tr>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer;\" onclick=\"sortList('brand', this)\" class=\"blue_text01 sorting\">브랜드 <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">출력대분류</th>";
    $list .= "\n      <th class=\"bm2px\">계열</th>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer\" onclick=\"sortList('name', this)\" class=\"blue_text01 sorting\">출력명  <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">판</th>";
    $list .= "\n      <th class=\"bm2px\">가로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">세로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">기준단위</th>";
    $list .= "\n      <th class=\"bm2px\">조회</th>";
    $list .= "\n  </tr>";

    $ret .= sprintf($list, $param["brand_sort"], $param["brand_sort_type"],
                    $param["name_sort"], $param["name_sort_type"]);

    return $ret; 
}

/*
 * 매입업체 THEAD
 * 
 * return : list
 */
function makePrintThead($param) {

    $ret = "";

    $list .= "\n  <tr>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer\" onclick=\"sortList('brand', this)\" class=\"blue_text01 sorting\">브랜드 <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">인쇄대분류</th>";
    $list .= "\n      <th class=\"bm2px\">계열</th>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer\" onclick=\"sortList('name', this)\" class=\"blue_text01 sorting\">인쇄명  <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">인쇄색도</th>";
    $list .= "\n      <th class=\"bm2px\">가로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">세로사이즈</th>";
    $list .= "\n      <th class=\"bm2px\">기준단위</th>";
    $list .= "\n      <th class=\"bm2px\">조회</th>";
    $list .= "\n  </tr>";

    $ret .= sprintf($list, $param["brand_sort"], $param["brand_sort_type"],
                    $param["name_sort"], $param["name_sort_type"]);

    return $ret; 
}

/*
 * 매입업체 THEAD
 * 
 * return : list
 */
function makeAfterThead($param) {

    $ret = "";

    $list .= "\n  <tr>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer\" onclick=\"sortList('brand', this)\" class=\"blue_text01 sorting\">브랜드 <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\"><a style=\"cursor:pointer\" onclick=\"sortList('name', this)\" class=\"blue_text01 sorting\">후공정명  <i class=\"fa %s %s\"></i></a></th>";
    $list .= "\n      <th class=\"bm2px\">depth1</th>";
    $list .= "\n      <th class=\"bm2px\">depth2</th>";
    $list .= "\n      <th class=\"bm2px\">depth3</th>";
    $list .= "\n      <th class=\"bm2px\">기준단위</th>";
    $list .= "\n      <th class=\"bm2px\">조회</th>";
    $list .= "\n  </tr>";

    $ret .= sprintf($list, $param["brand_sort"], $param["brand_sort_type"],
                    $param["name_sort"], $param["name_sort_type"]);

    return $ret; 
}

/*
 * 매입업체 종이 리스트
 * 
 * return : list
 */
function makePaperParam($result) {
    
    $param = array();

    $param["affil"] = $result->fields["affil"];
    $param["wid_size"] = $result->fields["wid_size"];
    $param["vert_size"] = $result->fields["vert_size"];
    $param["crtr_unit"] = $result->fields["crtr_unit"];
    $param["brand"] = $result->fields["brand"];
    $param["sort"] = $result->fields["sort"];
    $param["name"] = $result->fields["name"];
    $param["manu"] = $result->fields["manu_name"];
    $param["dvs"] = $result->fields["dvs"];
    $param["color"] = $result->fields["color"];
    $param["basisweight"] = $result->fields["basisweight"];
    $param["paper_seqno"] = $result->fields["paper_seqno"];
    $param["basic_price"] = $result->fields["basic_price"];
    $param["pur_rate"] = $result->fields["pur_rate"];
    $param["pur_aplc_price"] = $result->fields["pur_aplc_price"];
    $param["pur_price"] = $result->fields["pur_price"];

    return $param; 
}

/*
 * 매입업체 출력 리스트
 * 
 * return : list
 */
function makeOutputParam($result) {
    
    $param = array();

    $param["manu"] = $result->fields["manu_name"];
    $param["brand"] = $result->fields["brand"];
    $param["top"] = $result->fields["top"];
    $param["board"] = $result->fields["board"];
    $param["affil"] = $result->fields["affil"];
    $param["name"] = $result->fields["name"];
    $param["wid_size"] = $result->fields["wid_size"];
    $param["vert_size"] = $result->fields["vert_size"];
    $param["crtr_unit"] = $result->fields["crtr_unit"];
    $param["basic_price"] = $result->fields["basic_price"];
    $param["pur_rate"] = $result->fields["pur_rate"];
    $param["pur_aplc_price"] = $result->fields["pur_aplc_price"];
    $param["pur_price"] = $result->fields["pur_price"];

    return $param; 
}

/*
 * 매입업체 인쇄 리스트
 * 
 * return : list
 */
function makePrintParam($result) {
    
    $param = array();

    $param["manu"] = $result->fields["manu_name"];
    $param["brand"] = $result->fields["brand"];
    $param["top"] = $result->fields["top"];
    $param["tmpt"] = $result->fields["crtr_tmpt"];
    $param["affil"] = $result->fields["affil"];
    $param["name"] = $result->fields["name"];
    $param["wid_size"] = $result->fields["wid_size"];
    $param["vert_size"] = $result->fields["vert_size"];
    $param["crtr_unit"] = $result->fields["crtr_unit"];
    $param["basic_price"] = $result->fields["basic_price"];
    $param["pur_rate"] = $result->fields["pur_rate"];
    $param["pur_aplc_price"] = $result->fields["pur_aplc_price"];
    $param["pur_price"] = $result->fields["pur_price"];

    return $param; 
}

/*
 * 매입업체 후공정 리스트
 * 
 * return : list
 */
function makeAfterOptParam($result) {
    
    $param = array();

    $param["manu"] = $result->fields["manu_name"];
    $param["brand"] = $result->fields["brand"];
    $param["name"] = $result->fields["name"];
    $param["depth1"] = $result->fields["depth1"];
    $param["depth2"] = $result->fields["depth2"];
    $param["depth3"] = $result->fields["depth3"];
    $param["crtr_unit"] = $result->fields["crtr_unit"];
    $param["pur_prdt"] = $result->fields["pur_prdt"];
    $param["amt"] = $result->fields["amt"];
    $param["basic_price"] = $result->fields["basic_price"];
    $param["pur_rate"] = $result->fields["pur_rate"];
    $param["pur_aplc_price"] = $result->fields["pur_aplc_price"];
    $param["pur_price"] = $result->fields["pur_price"];

    return $param; 
}

/*
 * 매입업체 회원 리스트
 * 
 * return : list
 */
function makeExtnlMemberList($result) {
    
    $ret = "";
    $i = 1;

    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='7'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while ($result && !$result->EOF) {

        $param = array();
        $mng = $result->fields["mng"];
        $extnl_etprs_seqno = $result->fields["extnl_etprs_seqno"];
        $extnl_member_seqno = $result->fields["extnl_etprs_member_seqno"];
        $id = $result->fields["id"];
        $access_code = $result->fields["access_code"];
        $tel_num = $result->fields["tel_num"];
        $cell_num = $result->fields["cell_num"];
        $mail = $result->fields["mail"];
        $resp_task = $result->fields["resp_task"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td><button class=\"orge btn_pu btn fix_height20 fix_width40\" onclick=\"openMngAddPopup('mng' ,%d); return false;\">수정</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $mng, $id, $access_code,
                        $tel_num, $cell_num, $mail, $extnl_member_seqno);

        $result->moveNext();
    }

    return $ret; 
}

/*
 * 브랜드 list 생성
 * 
 * return : list
 */
function makeBrandList($result) {

    $ret = "";
    $i = 1;

    $cnt = $result->recordCount();

    if ($cnt == 0) {

        $ret = "\n   <tr><td colspan='3'>검색된 내용이 없습니다.</td></tr>";

        return $ret;

    }

    while($result && !$result->EOF) {

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td>%s</td>";
        $list .= "\n      <td><button type=\"button\" class=\"orge btn_pu btn fix_height20 fix_width40\" onclick=\"openMngAddPopup('brand', '%s');\">수정</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, 
                $i,
                $result->fields["name"],
                $result->fields["extnl_brand_seqno"]);

        $i++;
        $result->moveNext();
    }

    return $ret;
}


?>