<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

//지시서 내용 리스트
function makeOrdList($conn, $dao, $param) {

    $rs = $dao->selectProduceOrdList($conn, $param);

    $html = "";
    $list_html  = "\n                                           <tr class=\"%s\">";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                               <td>%s</td>";
    $list_html .= "\n                                           </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        $amt = "";
        if ($rs->fields["amt"] < 1 && $rs->fields["amt"] > 0) {
            $amt = number_format($rs->fields["amt"], 1);
        } else {
            $amt = number_format($rs->fields["amt"]);
        }

        $html .= sprintf($list_html, $class, $i
                ,$rs->fields["typset_num"]
                ,$rs->fields["paper"]
                ,$rs->fields["size"]
                ,$rs->fields["print_tmpt"]
                ,$amt . "장"
                ,$rs->fields["specialty_items"]);
        $i++;
        $rs->moveNext();
    }

    return $html;
}

// 구분별 지시서 생성
function makeOrdDvs($conn, $dao, $param, $dvs_rs) {

    $html = "";
    while ($dvs_rs && !$dvs_rs->EOF) {

        $dvs = $dvs_rs->fields["dvs"];
        $param["dvs"] = $dvs;

        $tot_rs = $dao->selectProduceOrdDvs($conn, $param);
        $cnt = number_format($tot_rs->fields["cnt"]);
        $tot_amt = "";
        if ($tot_rs->fields["tot_amt"] < 1 && $tot_rs->fields["tot_amt"] > 0) {
            $tot_amt = number_format($tot_rs->fields["tot_amt"], 1);
        } else {
            $tot_amt = number_format($tot_rs->fields["tot_amt"]);
        }

        $html .= "\n                                 <table class=\"table_basic_layout\">";
        $html .= "\n                                    <colgroup>";
        $html .= "\n                                        <col style=\"width:50px;\">";
        $html .= "\n                                        <col style=\"width:140px;\">";
        $html .= "\n                                        <col style=\"width:210px;\">";
        $html .= "\n                                        <col style=\"width:100px;\">";
        $html .= "\n                                        <col style=\"width:100px;\">";
        $html .= "\n                                        <col style=\"width:150px;\">";
        $html .= "\n                                        <col style=\"width:150px;\">";
        $html .= "\n                                    </colgroup>";
        $html .= "\n                                       <thead>";
        $html .= "\n                                           <tr>";
        $html .= "\n                                               <th colspan=\"7\" class=\"th_table_accent\">" . $dvs . " (" . substr($param["date"], 0 ,10) . ")</th>";
        $html .= "\n                                           </tr>";
        $html .= "\n                                           <tr>";
        $html .= "\n                                               <th class=\"bm2px\">번호</th>";
        $html .= "\n                                               <th class=\"bm2px\">조판번호</th>";
        $html .= "\n                                               <th class=\"bm2px\">종이</th>";
        $html .= "\n                                               <th class=\"bm2px\">사이즈</th>";
        $html .= "\n                                               <th class=\"bm2px\">도수</th>";
        $html .= "\n                                               <th class=\"bm2px\">수량</th>";
        $html .= "\n                                               <th class=\"bm2px\">특기사항</th>";
        $html .= "\n                                           </tr>";
        $html .= "\n                                       </thead>";
        $html .= "\n                                       <tbody>";
        $html .= makeOrdList($conn, $dao, $param);
        $html .= "\n                                       </tbody>";
        $html .= "\n                                       <thead>";
        $html .= "\n                                           <tr>";
        $html .= "\n                                               <th colspan=\"5\"></th>";
        $html .= "\n                                               <th>판수량 합계 : " . $cnt . "</th>";
        $html .= "\n                                               <th>판매수 합계 : " . $tot_amt . "장</th>";
        $html .= "\n                                           </tr>";
        $html .= "\n                                       </thead>";
        $html .= "\n                                 </table>";

        $dvs_rs->moveNext();
    }

    return $html;
}

// 생산 지시서 생성
function makeProduceOrd($conn, $dao, $param) {
    $ord_dvs_rs = $dao->selectOrdDvs($conn, $param);

    $html = "";
    while ($ord_dvs_rs && !$ord_dvs_rs->EOF) {

       //$ord_dvs = $ord_dvs_rs->fields["ord_dvs"];
       //$param["ord_dvs"] = $ord_dvs;

       $dvs_rs = $dao->selectDvs($conn, $param);

       $html .= "\n                                 <table class=\"table_basic_layout\">";
       $html .= "\n                                     <thead>";
       $html .= "\n                                         <tr><th style=\"background-color: #ddd; font-weight:600;\">생산지시서 [" . $param["ord_dvs"] . "]</th></tr>";
       $html .= "\n                                     </thead>";
       $html .= "\n                                 </table>";
       $html .= makeOrdDvs($conn, $dao, $param, $dvs_rs);
       $html .= "\n                                 <br />";

       $ord_dvs_rs->moveNext();
   }

   return $html;
}

// 생산지시서 총합 리스트
function makeTotalList($conn, $dao, $param) {

    $dvs_arr = array();
    $dvs_arr[0] = "명함";
    $dvs_arr[1] = "스티커";
    $dvs_arr[2] = "전단";

    $td_html  = "";
    $td_html .= "<td>%s</td>";
    $td_html .= "<td>%s</td>";
    $td_html .= "<td>%s</td>";

    $i = 0;
    $th = "";
    $th2 = "";
    $board_arr = array();
    $page_arr = array();
    $b_amt = "";
    $p_amt = "";
    foreach ($dvs_arr as $key => $value) {

        $th .= "\n<th colspan=\"" . count($dvs_arr) . "\">" . $value . "</th>";
        $th2 .= "<th>당일판</th><th>본판</th><th>전체</th>";
        $param["dvs"] = $dvs_arr[$i];
        $rs = $dao->selectTotalList($conn, $param);

        $rs_cnt = 0;
        $board_arr = array();
        $page_arr = array();
        while ($rs && !$rs->EOF) {
            $board_arr[$rs->fields["dvs"]] = (int)$board_arr[$rs->fields["dvs"]] + 1;
            $page_arr[$rs->fields["dvs"]] = (int)$page_arr[$rs->fields["dvs"]] + (int)$rs->fields["print_amt"];

            $rs_cnt++;
            $rs->moveNext();
        }
        if($rs_cnt == 0) {
            $b_amt .= sprintf($td_html, 0,
                0,
                0);
            $p_amt .= sprintf($td_html, 0,
                0,
                0);
        } else {
            $b_amt .= sprintf($td_html, number_format($board_arr["당일판"]),
                number_format($board_arr[$value]),
                number_format((int)$board_arr[$value]));
            $p_amt .= sprintf($td_html, number_format($page_arr["당일판"]),
                number_format($page_arr[$value]),
                number_format((int)$page_arr[$value]));
        }

        $i++;
    }

//    $html = "<br />";
    $html  = "\n<table class=\"table_basic_layout\">";
    $html .= "\n  <thead>";
    $html .= "\n    <tr>";
    $html .= "\n      <th rowspan=\"2\"></th>";
    $html .= $th;
    $html .= "\n    </tr>";
    $html .= "\n    <tr>";
    $html .= $th2;
    $html .= "\n    </tr>";
    $html .= "\n  </thead>";
    $html .= "\n  <tbody>";
    $html .= "\n    <tr>";
    $html .= "\n      <th style=\"background: #eee;\">판수</th>";
    $html .= $b_amt;
    $html .= "\n    </tr>";
    $html .= "\n    <tr>";
    $html .= "\n      <th style=\"background: #eee;\">장수</th>";
    $html .= $p_amt;
    $html .= "\n    </tr>";
    $html .= "\n  </tbody>";
    $html .= "\n</table>";

    return $html;
}

function makeProduceListByPrinterHtml($rs) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n    <td class='font_size'>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        $method = "";
        if($rs->fields["honggak_yn"] == "N")
            $method = "돈땡";
        else {
            if($rs->fields["prdt_page"] == 1)
                $method = "단면";

            if($rs->fields["prdt_page"] == 2)
                $method = "양면";
        }

        $rs_html .= sprintf($html, $class,
            $i,
            $rs->fields["typset_num"],
            $paper_name,
            $rs->fields["size"],
            ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도",
            $method,
            $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
            "서울판",
            $rs->fields["after_name"],
            $rs->fields["memo"]);
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}


function makeProduceListByNamecardHtml($rs) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    $print_amt = 0;
    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }
        $size = $rs->fields["size"];
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        $rs_html .= sprintf($html, $class,
            $i,
            $rs->fields["typset_num"],
            $paper_name,
            $rs->fields["amt"],
            $rs->fields["memo"]);
        $i++;
        $print_amt += $rs->fields["print_amt"];
        $rs->moveNext();
    }

    $report = "판 합계 : " . $i . " 매수 합계 : " . $print_amt;
    $param = array();
    $param["html"] = $rs_html;
    $param["report"] = $report;
    return $param;
}

function makeProduceListByPaperHtml($rs) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }
        $size = $rs->fields["size"];
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        $rs_html .= sprintf($html, $class,
            $i,
            $paper_name,
            $size,
            $rs->fields["amt"]);
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeProduceListByAfterHtml($rs) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }
        /*
        <option value="">전체</option>
                        <option value="택배">택배</option>
                        <option value="직배송">직배송</option>
                        <option value="퀵">퀵</option>
        */
        $rs_html .= sprintf($html, $class,
            $i,
            $rs->fields["typset_num"],
            $rs->fields["member_name"],
            $rs->fields["title"],
            $paper_name,
            $rs->fields["stan_name"],
            $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
            ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도",
            $rs->fields["after_name"],
            $rs->fields["memo"],
            $rs->fields["print_etprs"],
            "서울");
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeProduceListByEnvelopeHtml($rs) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        $dlvr_way = "";
        if($rs->fields["dlvr_way"] == "01")
            $dlvr_way = "택배";
        else if($rs->fields["dlvr_way"] == "02")
            $dlvr_way = $rs->fields["dlvr_add_info"];
        else if($rs->fields["dlvr_way"] == "03")
            $dlvr_way = "화물";
        else if($rs->fields["dlvr_way"] == "04")
            $dlvr_way = "오토바이";
        else if($rs->fields["dlvr_way"] == "05")
            $dlvr_way = "지하철";
        else if($rs->fields["dlvr_way"] == "06")
            $dlvr_way = "인현동방문";
        else if($rs->fields["dlvr_way"] == "07")
            $dlvr_way = "성수동방문";
        /*
        <option value="">전체</option>
                        <option value="택배">택배</option>
                        <option value="직배송">직배송</option>
                        <option value="퀵">퀵</option>
        */

        $stan = $rs->fields["stan_name"];
        if(strpos($stan, "175*85") !== false) {
            $stan = "티켓";
        }
        if(strpos($stan, "220*105") !== false) {
            $detail = explode(" / ", $rs->fields["order_detail"])[0];
            if(strpos($stan,"창문") !== false) {
                $stan = "창봉투";
            }
            else if(strpos($detail,"자켓형") !== false) {
                $stan = "자켓";
            } else if(strpos($detail,"일반형") !== false) {
                $stan = "A4소";
            }
        }
        if(strpos($stan, "330*245") !== false) {
            $stan = "대봉";
        }
        $rs_html .= sprintf($html, $class,
            $i,
            $rs->fields["typset_num"],
            $dlvr_way,
            $rs->fields["member_name"],
            $rs->fields["title"],
            explode(" / ", $rs->fields["order_detail"])[0],
            $paper_name,
            $stan,
            number_format($rs->fields["amt"]),
            number_format($rs->fields["count"]) . "건",
            //number_format($rs->fields["print_amt"]) . $rs->fields["print_amt_unit"],
            ($rs->fields["after_name"] == "양면테입" || $rs->fields["after_name"] == "양면테이프") ? "O" : "",
            $rs->fields["work_memo"]);
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeProduceListByCommercialHtml($rs, $conn, $dao) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        $dlvr_way = "";
        if($rs->fields["dlvr_way"] == "01")
            $dlvr_way = "택배";
        else if($rs->fields["dlvr_way"] == "02")
            $dlvr_way = $rs->fields["dlvr_add_info"];
        else if($rs->fields["dlvr_way"] == "03")
            $dlvr_way = "화물";
        else if($rs->fields["dlvr_way"] == "04")
            $dlvr_way = "오토바이";
        else if($rs->fields["dlvr_way"] == "05")
            $dlvr_way = "지하철";
        else if($rs->fields["dlvr_way"] == "06")
            $dlvr_way = "인현동방문";
        else if($rs->fields["dlvr_way"] == "07")
            $dlvr_way = "성수동방문";
        /*
        <option value="">전체</option>
                        <option value="택배">택배</option>
                        <option value="직배송">직배송</option>
                        <option value="퀵">퀵</option>
        */

        $stan = $rs->fields["stan_name"];
        if(strpos($stan, "175*85") !== false) {
            $stan = "티켓";
        }
        if(strpos($stan, "220*105") !== false) {
            $detail = explode(" / ", $rs->fields["order_detail"])[0];
            if(strpos($stan,"창문") !== false) {
                $stan = "창봉투";
            }
            else if(strpos($detail,"자켓형") !== false) {
                $stan = "자켓";
            } else if(strpos($detail,"일반형") !== false) {
                $stan = "A4소";
            }
        }
        if(strpos($stan, "330*245") !== false) {
            $stan = "대봉";
        }
        $amt = $rs->fields["amt"];
        if(strpos($stan, "2단 리플렛") !== false
        || strpos($stan, "3단 리플렛") !== false
        || strpos($stan, "대문접지리플렛") !== false) {
            if($rs->fields["cut_size_wid"] == "182" && $rs->fields["cut_size_vert"] == "257") {
                $stan = "16절";
                $amt /= 8000;
            }
            if($rs->fields["cut_size_wid"] == "210" && $rs->fields["cut_size_vert"] == "297") {
                $stan = "A4";
                $amt /= 4000;
            }
            if($rs->fields["cut_size_wid"] == "367" && $rs->fields["cut_size_vert"] == "257") {
                $stan = "8절";
                $amt /= 4000;
            }
            if($rs->fields["cut_size_wid"] == "423" && $rs->fields["cut_size_vert"] == "297") {
                $stan = "A3";
                $amt /= 2000;
            }
            if($rs->fields["cut_size_wid"] == "420" && $rs->fields["cut_size_vert"] == "297") {
                $stan = "A3";
                $amt /= 2000;

            }
            if($rs->fields["cut_size_wid"] == "619" && $rs->fields["cut_size_vert"] == "297") {
                $stan = "국3절";
                $amt /= 666;
                $amt = ceil($amt * 100) / 100;
            }
            if($rs->fields["cut_size_wid"] == "840" && $rs->fields["cut_size_vert"] == "297") {
                $stan = "장국2절";
                $amt /= 1000;
            }

            //echo $amt . "<br>";
        }
        $amt = $amt / 10 * 10;
        $param['order_common_seqno'] = $rs->fields["order_common_seqno"];

        $rs_html .= sprintf($html, $class,
            $i,
            explode('-', $rs->fields["typset_num"])[0] . "-" . explode('-', $rs->fields["typset_num"])[1],
            $dlvr_way,
            $rs->fields["member_name"],
            $rs->fields["title"],
            explode(" / ", $rs->fields["order_detail"])[0],
            $paper_name,
            $stan,
            $amt . "연(" . $rs->fields["page_cnt"] . '매)',
            //number_format($rs->fields["print_amt"]) . $rs->fields["print_amt_unit"],
            $dao->selectOrderAfterInfo2($conn, $param),
            $rs->fields["work_memo"],
            $rs->fields["print_etprs"]);
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeProduceListByTypsetHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        if($rs->fields["after_name"] == "") {
            $rs_html .= sprintf($html, $class,
                $i,
                $rs->fields["typset_num"],
                "롯데택배",
                $rs->fields["member_name"],
                $rs->fields["title"],
                $paper_name,
                $rs->fields["stan_name"],
                ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도",
                $rs->fields["count"],
                $rs->fields["after_name"],
                $rs->fields["memo"],
                $rs->fields["print_etprs"],
                "서울");
        }
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeProduceListByTypsetDirectHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";
    $i = 1;

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }
        $paper_name = $rs->fields["paper_name"];
        if($rs->fields["paper_name"] == "아트지") {
            $paper_name = "아트지 100g";
        }

        if($rs->fields["after_name"] == "") {
            $rs_html .= sprintf($html, $class,
                $i,
                $rs->fields["typset_num"],
                $rs->fields["dlvr_add_info"],
                $rs->fields["member_name"],
                $rs->fields["title"],
                $paper_name,
                $rs->fields["stan_name"],
                ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도",
                $rs->fields["count"],
                $rs->fields["after_name"],
                $rs->fields["memo"],
                $rs->fields["print_etprs"],
                "서울");
        }
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

function getEnvelopeValue($key)
{
    $size = "";
    switch ($key) {
        case "국전-대봉2개-소봉4개|모조지":
        case "국전-대봉2개-소봉4개|100 모조":
        case "국전-대봉2개-소봉4개|120 모조":
        case "국전-대봉2개-소봉4개|150 모조":
        case "8색기-국전-대봉2개-소봉4개|모조지":
        case "8색기-국전-대봉2개-소봉4개|100 모조":
        case "8색기-국전-대봉2개-소봉4개|120 모조":
        case "8색기-국전-대봉2개-소봉4개|150 모조":
            $size = "1020 x 670";
            break;
        case "국전-대봉2개-소봉4개|레자크 #91(체크)":
        case "국전-대봉2개-소봉4개|#91 백색 레쟈크":
        case "8색기-국전-대봉2개-소봉4개|레자크 #91(체크)":
        case "8색기-국전-대봉2개-소봉4개|#91 백색 레쟈크":
            $size = "1020 x 670";
        break;
        case "국전-대봉2개-소봉4개|레자크 #92(줄)":
        case "국전-대봉2개-소봉4개|#92 백색 레쟈크":
        case "8색기-국전-대봉2개-소봉4개|레자크 #92(줄)":
        case "8색기-국전-대봉2개-소봉4개|#92 백색 레쟈크":
            $size = "1020 x 670";
        break;

        case "2절-대봉2개|모조지":
        case "2절-대봉2개|100 모조":
        case "2절-대봉2개|120 모조":
        case "2절-대봉2개|150 모조":
        case "우성-2절-대봉2개|모조지":
        case "우성-2절-대봉2개|100 모조":
        case "우성-2절-대봉2개|120 모조":
        case "우성-2절-대봉2개|150 모조":
        case "8색기-2절-대봉2개|모조지":
        case "8색기-2절-대봉2개|100 모조":
        case "8색기-2절-대봉2개|120 모조":
        case "8색기-2절-대봉2개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉2개|레자크 #91(체크)":
        case "2절-대봉2개|#91 백색 레쟈크":
        case "우성-2절-대봉2개|레자크 #91(체크)":
        case "우성-2절-대봉2개|#91 백색 레쟈크":
        case "8색기-2절-대봉2개|레자크 #91(체크)":
        case "8색기-2절-대봉2개|#91 백색 레쟈크":
            $size = "506 x 788";break;
        case "2절-대봉2개|레자크 #92(줄)":
        case "2절-대봉2개|#92 백색 레쟈크":
        case "우성-2절-대봉2개|레자크 #92(줄)":
        case "우성-2절-대봉2개|#92 백색 레쟈크":
        case "8색기-2절-대봉2개|레자크 #92(줄)":
        case "8색기-2절-대봉2개|#92 백색 레쟈크":
            $size = "506 x 788";break;


        case "국전-소봉투(12개)|모조지":
        case "국전-소봉투(12개)|100 모조":
        case "국전-소봉투(12개)|120 모조":
        case "국전-소봉투(12개)|150 모조":
        case "8색기-국전-소봉투(12개)|모조지":
        case "8색기-국전-소봉투(12개)|100 모조":
        case "8색기-국전-소봉투(12개)|120 모조":
        case "8색기-국전-소봉투(12개)|150 모조":
            $size = "1060 x 740";break;
        case "국전-소봉투(12개)|레자크 #91(체크)":
        case "국전-소봉투(12개)|#91 백색 레쟈크":
        case "8색기-국전-소봉투(12개)|레자크 #91(체크)":
        case "8색기-국전-소봉투(12개)|#91 백색 레쟈크":
            $size = "1060 x 740";break;
        case "국전-소봉투(12개)|레자크 #92(줄)":
        case "국전-소봉투(12개)|#92 백색 레쟈크":
        case "8색기-국전-소봉투(12개)|레자크 #92(줄)":
        case "8색기-국전-소봉투(12개)|#92 백색 레쟈크":
            $size = "1060 x 740";break;


        case "2절-대봉1개-소봉2개|모조지":
        case "2절-대봉1개-소봉2개|100 모조":
        case "2절-대봉1개-소봉2개|120 모조":
        case "2절-대봉1개-소봉2개|150 모조":
        case "우성-2절-대봉1개-소봉2개|모조지":
        case "우성-2절-대봉1개-소봉2개|100 모조":
        case "우성-2절-대봉1개-소봉2개|120 모조":
        case "우성-2절-대봉1개-소봉2개|150 모조":
        case "8색기-2절-대봉1개-소봉2개|모조지":
        case "8색기-2절-대봉1개-소봉2개|100 모조":
        case "8색기-2절-대봉1개-소봉2개|120 모조":
        case "8색기-2절-대봉1개-소봉2개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "2절-대봉1개-소봉2개|#91 백색 레쟈크":
        case "우성-2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "우성-2절-대봉1개-소봉2개|#91 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "8색기-2절-대봉1개-소봉2개|#91 백색 레쟈크":
            $size = "506 x 788";break;
        case "2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "2절-대봉1개-소봉2개|#92 백색 레쟈크":
        case "우성-2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "우성-2절-대봉1개-소봉2개|#92 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "8색기-2절-대봉1개-소봉2개|#92 백색 레쟈크":
            $size = "788 x 506";break;


        case "2절-대봉1개-소봉1개-이삿짐봉투1개|모조지":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|100 모조":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|120 모조":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|150 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|모조지":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|100 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|120 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #91(체크)":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|#91 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #91(체크)":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|#91 백색 레쟈크":
            $size = "788 x 506";break;
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #92(줄)":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|#92 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #92(줄)":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|#92 백색 레쟈크":
            $size = "788 x 506";break;


        case "2절-소봉6개|모조지":
        case "2절-소봉6개|100 모조":
        case "2절-소봉6개|120 모조":
        case "2절-소봉6개|150 모조":
        case "8색기-2절-소봉6개|모조지":
        case "8색기-2절-소봉6개|100 모조":
        case "8색기-2절-소봉6개|120 모조":
        case "8색기-2절-소봉6개|150 모조":
            $size = "788 x 545";break;
        case "2절-소봉6개|레자크 #91(체크)":
        case "2절-소봉6개|#91 백색 레쟈크":
        case "8색기-2절-소봉6개|레자크 #91(체크)":
        case "8색기-2절-소봉6개|#91 백색 레쟈크":
            $size = "788 x 545";break;
        case "2절-소봉6개|레자크 #92(줄)":
        case "2절-소봉6개|#92 백색 레쟈크":
        case "8색기-2절-소봉6개|레자크 #92(줄)":
        case "8색기-2절-소봉6개|#92 백색 레쟈크":
            $size = "788 x 545";break;
        default:
            $size = "";
    }

    return $size;
}
?>
