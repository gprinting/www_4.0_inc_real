<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 그린백
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/BindingPriceUtil.inc");

class Catalog extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $dvs_arr = ["cover", "inner1", "inner2", "inner3"];

    var $flattyp_yn;
    var $amt;
    var $amt_unit;
    var $stan_mpcode;
    var $pos_num;
    var $affil;
    var $binding_mpcode;
    var $binding_depth1;
    var $size_name;

    var $paper_mpcode;
    var $print_bef_mpcode;
    var $print_aft_mpcode;
    var $print_bef_add_mpcode;
    var $print_aft_add_mpcode;
    var $print_bef_name;
    var $print_aft_name;
    var $print_bef_add_name;
    var $print_aft_add_name;
    var $page;
    var $page_detail;


    function makeHtml() {
        $html = '<h2>명함</h2></br></br>';
        //표지
        $html .= '-----------표지-----------</br>';
        $html .= $this->makeBasicOption(0);
        $html .= $this->makeOptOption();
        $html .= $this->makeAfterOption();
        $html .= $this->makeCommonInfo();
        return $html;
    }

    function setInfo($param)
    {
        $factory = new DPrintingFactory();
        $this->amt = $param["amt"];
        $this->amt_unit = $param["amt_unit"];
        $this->stan_mpcode = $param["size"];
        $this->size_name = $param["size_name"];
        $this->pos_num = $param["pos_num"];
        $this->affil = $param["affil"];
        $this->size_w = $param["cut_wid_size"];
        $this->size_h = $param["cut_vert_size"];
        $this->flattyp_yn = 'N';
        $this->count = 1;
        $this->binding_mpcode = $param["binding_mpcode"];
        $this->binding_depth1 = $param["binding_depth1"];
        if($param["prdt_dvs"] != null)
            $this->dvs_arr = explode('|', $param["prdt_dvs"]);
        $product = $this;
        foreach ($this->dvs_arr as $dvs) {
            $prefix = $dvs . '_';
            $this->paper_mpcode[$dvs] = $param[$prefix . "paper"];
            $this->print_bef_mpcode[$dvs] = $param[$prefix . "bef_tmpt"];
            $this->print_aft_mpcode[$dvs] = $param[$prefix . "aft_tmpt"];
            $this->print_bef_add_mpcode[$dvs] = $param[$prefix . "bef_add_tmpt"];
            $this->print_aft_add_mpcode[$dvs] = $param[$prefix . "aft_add_tmpt"];
            $this->print_bef_name[$dvs] = $param[$prefix . "bef_tmpt_name"];
            $this->print_aft_name[$dvs] = $param[$prefix . "aft_tmpt_name"];
            $this->print_bef_add_name[$dvs] = $param[$prefix . "bef_add_tmpt_name"];
            $this->print_aft_add_name[$dvs] = $param[$prefix . "aft_add_tmpt_name"];

            $page_info    = explode('!', $param[$prefix . "page_info"]);
            $this->page[$dvs] = $page_info[0];
            $this->page_detail[$dvs] = $page_info[1];

            $after_name = explode("|", $param[$prefix . "after_name"]);
            $aft_depth = explode("|", $param[$prefix . "aft_depth"]);
            $aft_depth1 = explode("|", $param[$prefix . "aft_depth1"]);
            $aft_depth2 = explode("|", $param[$prefix . "aft_depth2"]);
            $aft_depth1_val = explode("|", $param[$prefix . "aft_depth1_val"]);
            $aft_depth1_vh = explode("|", $param[$prefix . "aft_depth1_vh"]);
            $aft_depth1_cnt = explode("|", $param[$prefix . "aft_depth1_cnt"]);
            $aft_depth1_dvs = explode("|", $param[$prefix . "aft_depth1_dvs"]);
            $aft_depth2_dvs = explode("|", $param[$prefix . "aft_depth2_dvs"]);
            $aft_depth1_wid = explode("|", $param[$prefix . "aft_depth1_wid"]);
            $aft_depth1_vert = explode("|", $param[$prefix . "aft_depth1_vert"]);
            $aft_depth2_wid = explode("|", $param[$prefix . "aft_depth2_wid"]);
            $aft_depth2_vert = explode("|", $param[$prefix . "aft_depth2_vert"]);
            $aft_depth1_amt = explode("|", $param[$prefix . "aft_depth1_amt"]);

            $after_count = count($after_name);
            for($i = 0; $i < $after_count ; $i++) {
                if($after_name[$i] == '') {
                    break;
                }

                $product = $factory->createAP($after_name[$i], $product);
                $arr_depth = [];
                $arr_depth[$i]['after_name'] = $dvs . "_" . $after_name[$i];
                $arr_depth[$i]['aft_depth'] = $aft_depth[$i];
                $arr_depth[$i]['aft_depth1'] = $aft_depth1[$i];
                $arr_depth[$i]['aft_depth2'] = $aft_depth2[$i];
                $arr_depth[$i]['aft_depth1_val'] = $aft_depth1_val[$i];
                $arr_depth[$i]['aft_depth1_vh'] = $aft_depth1_vh[$i];
                $arr_depth[$i]['aft_depth1_cnt'] = $aft_depth1_cnt[$i];
                $arr_depth[$i]['aft_depth1_dvs'] = $aft_depth1_dvs[$i];
                $arr_depth[$i]['aft_depth2_dvs'] = $aft_depth2_dvs[$i];
                $arr_depth[$i]['aft_depth1_wid'] = $aft_depth1_wid[$i];
                $arr_depth[$i]['aft_depth1_vert'] = $aft_depth1_vert[$i];
                $arr_depth[$i]['aft_depth2_wid'] = $aft_depth2_wid[$i];
                $arr_depth[$i]['aft_depth2_vert'] = $aft_depth2_vert[$i];
                $arr_depth[$i]['aft_depth1_amt'] = $aft_depth1_amt[$i];
                $arr_depth[$i]['dvs'] = $dvs;
                $arr_depth[$i]['size_name'] = $this->size_name;
                $product->setAfterprocess($this->sortcode, $arr_depth[$i], $this->amt); // $sortcode ,$after_name, $mpcode, $amt = ''
            }
        }
        $product = $factory->createDC(null, $product);
        $product->grade = $param[grade];
        return $product;
    }

    function cost() {
        $calcUtil = new CalcPriceUtil();
        $util = new FrontCommonUtil();

        $temp = array();
        $temp["sell_site"]     = $this->sell_site;
        $temp["cate_sortcode"] = $this->sortcode;
        $temp["amt_unit"]      = $this->amt_unit;
        $temp["flattyp_yn"]    = $this->flattyp_yn;

        $temp["amt"]     = $this->amt;
        $temp["pos_num"] = $this->pos_num;
        $temp["affil"]   = $this->affil;
        $temp["size_name"]   = $this->size_name;

        $total_price = 0;

        $page = 0;
        foreach ($this->dvs_arr as $dvs) {
            $temp["paper_mpcode"]  = $this->paper_mpcode[$dvs];
            $temp["cate_paper_mpcode"]  = $this->paper_mpcode[$dvs];
            $temp["cate_output_mpcode"] = $this->stan_mpcode;

            $temp["bef_print_mpcode"]     = $this->print_bef_mpcode[$dvs];
            $temp["aft_print_mpcode"]     = $this->print_aft_mpcode[$dvs];
            $temp["bef_add_print_mpcode"] = $this->print_bef_add_mpcode[$dvs];
            $temp["aft_add_print_mpcode"] = $this->print_aft_add_mpcode[$dvs];
            $temp["page"] = $this->page[$dvs];
            $page += (int)$temp["page"];

            $calcUtil->setData($temp);

            $print_name_arr = array();
            $print_name_arr["bef_print_name"] = $this->print_bef_name[$dvs];
            $print_name_arr["aft_print_name"] = $this->print_aft_name[$dvs];
            $print_name_arr["bef_add_print_name"] = $this->print_bef_add_name[$dvs];
            $print_name_arr["aft_add_print_name"] = $this->print_aft_add_name[$dvs];

            $paper_price  = $util->ceilVal($calcUtil->calcPaperPrice($print_name_arr)) * $this->discount_rate;
            $print_price  = $util->ceilVal($calcUtil->calcBookletPrintPrice()) * $this->discount_rate;
            $output_price = $util->ceilVal($calcUtil->calcBookletOutputPrice()) * $this->discount_rate;
            $sell_price   = $paper_price + $print_price + $output_price;

            $price_arr[$dvs]["paper"]  = $paper_price;
            $price_arr[$dvs]["output"] = $output_price;
            $price_arr[$dvs]["print"]  = $print_price;
            $price_arr[$dvs]["sell_price"]   = $sell_price;
            $total_price += $sell_price;
        }

        //20 제본 가격 계산
        $param = array();
        $param["mpcode"]    = $this->binding_mpcode;
        $param["amt"]       = $this->amt;

        $binding_price = $this->dao->selectBindingPrice($this->conn, $param);
        $binding_price = intval($binding_price);

        $param["cate_sortcode"] = "001001001";
        $param["amt"]           = $this->amt; //V
        $param["page"]          = $page; //V
        $param["price"]         = $binding_price;
        $param["coating_yn"]    = false;
        $param["depth1"]        = $this->binding_depth1;
        //$param["stan_name"]     = $price_info_arr["size_name"];
        //$param["pos_num"]       = $price_info_arr["pos_num"];

        //$bindingPriceUtil = new BindingPriceUtil($param);
        //$binding_price = $bindingPriceUtil->calcBindingPrice();
        //echo $binding_price;

        if($this->binding_depth1 == "중철제본") {
            $page /= 2;
            $per_price_binding = 0;
            if($page <= 10) {
                if($this->amt < 2) {
                    $per_price_binding = 2000;
                } else if($this->amt < 10) {
                    $per_price_binding = 1500;
                } else if($this->amt < 30) {
                    $per_price_binding = 1200;
                } else if($this->amt < 50) {
                    $per_price_binding = 800;
                } else if($this->amt < 100) {
                    $per_price_binding = 500;
                } else {
                    $per_price_binding = 300;
                }
            } else if($page < 20) {
                if($this->amt < 2) {
                    $per_price_binding = 2500;
                } else if($this->amt < 10) {
                    $per_price_binding = 2000;
                } else if($this->amt < 30) {
                    $per_price_binding = 1500;
                } else if($this->amt < 50) {
                    $per_price_binding = 1000;
                } else if($this->amt < 100) {
                    $per_price_binding = 700;
                } else {
                    $per_price_binding = 400;
                }
            } else {
                if($this->amt < 2) {
                    $per_price_binding = 3000;
                } else if($this->amt < 10) {
                    $per_price_binding = 2500;
                } else if($this->amt < 30) {
                    $per_price_binding = 2200;
                } else if($this->amt < 50) {
                    $per_price_binding = 1500;
                } else if($this->amt < 100) {
                    $per_price_binding = 1000;
                } else {
                    $per_price_binding = 600;
                }
            }

            $binding_price = $per_price_binding * $this->amt;
        }

        if($this->binding_depth1 == "무선제본") {
            $page /= 2;
            $per_price_binding = 0;
            if($page < 100) {
                if($this->amt < 2) {
                    $per_price_binding = 2000;
                } else if($this->amt < 10) {
                    $per_price_binding = 1500;
                } else if($this->amt < 30) {
                    $per_price_binding = 1000;
                } else if($this->amt < 50) {
                    $per_price_binding = 700;
                } else if($this->amt < 100) {
                    $per_price_binding = 500;
                } else {
                    $per_price_binding = 400;
                }
            } else if($page < 400) {
                if($this->amt < 2) {
                    $per_price_binding = 2000;
                } else if($this->amt < 10) {
                    $per_price_binding = 1500;
                } else if($this->amt < 30) {
                    $per_price_binding = 1000;
                } else if($this->amt < 50) {
                    $per_price_binding = 800;
                } else if($this->amt < 100) {
                    $per_price_binding = 600;
                } else {
                    $per_price_binding = 500;
                }
            } else {
                if($this->amt < 2) {
                    $per_price_binding = 3000;
                } else if($this->amt < 10) {
                    $per_price_binding = 2000;
                } else if($this->amt < 30) {
                    $per_price_binding = 1500;
                } else if($this->amt < 50) {
                    $per_price_binding = 1200;
                } else if($this->amt < 100) {
                    $per_price_binding = 1000;
                } else {
                    $per_price_binding = 800;
                }
            }

            $binding_price = $per_price_binding * $this->amt;
        }

        if($this->binding_depth1 == "스프링제본") {
            $page /= 2;
            $per_price_binding = 0;
            if($page < 100) {
                if($this->amt < 2) {
                    $per_price_binding = 1500;
                } else if($this->amt < 10) {
                    $per_price_binding = 1500;
                } else if($this->amt < 30) {
                    $per_price_binding = 1500;
                } else if($this->amt < 50) {
                    $per_price_binding = 1200;
                } else if($this->amt < 100) {
                    $per_price_binding = 1000;
                } else {
                    $per_price_binding = 800;
                }
            } else if($page < 400) {
                if($this->amt < 2) {
                    $per_price_binding = 2500;
                } else if($this->amt < 10) {
                    $per_price_binding = 2000;
                } else if($this->amt < 30) {
                    $per_price_binding = 1800;
                } else if($this->amt < 50) {
                    $per_price_binding = 1500;
                } else if($this->amt < 100) {
                    $per_price_binding = 1200;
                } else {
                    $per_price_binding = 1000;
                }
            } else {
                if($this->amt < 2) {
                    $per_price_binding = 3000;
                } else if($this->amt < 10) {
                    $per_price_binding = 2500;
                } else if($this->amt < 30) {
                    $per_price_binding = 2200;
                } else if($this->amt < 50) {
                    $per_price_binding = 2000;
                } else if($this->amt < 100) {
                    $per_price_binding = 1700;
                } else {
                    $per_price_binding = 1500;
                }
            }
        }
        //echo $per_price_binding . " aa" . $this->amt;
        $binding_price = $per_price_binding * $this->amt;
        if($this->amt < 1000) {
            $binding_price *= 0.3;
        } else if($this->amt < 5000) {
            $binding_price *= 0.25;
        } else if($this->amt < 10000) {
            $binding_price *= 0.2;
        } else if($this->amt < 20000) {
            $binding_price *= 0.18;
        } else if($this->amt < 30000) {
            $binding_price *= 0.16;
        } else if($this->amt < 40000) {
            $binding_price *= 0.14;
        } else {
            $binding_price *= 0.12;
        }
        if($binding_price < 100000) $binding_price = 100000;


        $binding_price *= 1.1;
        $price_arr["sell_price"] = $total_price + $binding_price;
        $price_arr["binding_price"] = $binding_price;
        return $price_arr;
    }

    function getJarisu() {
        return $this->jarisu;
    }
    function getSort() {
        return "leaflet";
    }

    function makeOrderCommonInsertParam($fb, $session) {
        $price = $this->cost();
        $frontUtil = new FrontCommonUtil();
        $this->order_common_param = array();
        if($fb["order_common_seqno"] != '') {
            $this->order_common_param["order_common_seqno"] = $fb["order_common_seqno"];
            $order_common_seqno = $fb["order_common_seqno"];
        }

        $common_prdt_dvs = $fb["common_prdt_dvs"];
        $dvs = $common_prdt_dvs . "_";

        $this->order_common_param["amt"]           = $fb["amt"];
        $this->order_common_param["amt_unit_dvs"]  = $fb["amt_unit"];
        $this->order_common_param["order_detail"]  = $fb[$dvs . "order_detail"];
        $this->order_common_param["group_seqno"] = 0;
        $this->order_common_param["del_yn"] = 'N';
        $this->order_common_param["count"] = $fb["count"];
        $this->order_common_param["title"] = $fb["title"];
        $this->order_common_param["cust_memo"] = $fb["cust_memo"];
        $this->order_common_param["member_seqno"] = $session["org_member_seqno"];
        $this->order_common_param["order_state"] = "1120";
        $this->order_common_param["claim_yn"] = 'N';
        $this->order_common_param["point_use_yn"] = 'N';
        $this->order_common_param["oper_sys"] = $fb["oper_sys"];
        $this->order_common_param["file_upload_dvs"] = 'Y';
        $this->order_common_param["add_opt_price"] = $frontUtil->rmComma($fb["opt_price"]);
        $this->order_common_param["event_price"] = $fb["event_price"];
        $this->order_common_param["event_yn"] = $fb["event_yn"];
        $this->order_common_param["dlvr_dvs"] = $this->getSort();
        if($this->order_common_param["event_yn"] == null) $this->order_common_param["event_yn"] = "N";

        $prdt_dvs = $fb["prdt_dvs"];
        $arr_common_prdt_dvs = explode("|", $prdt_dvs);
        foreach ($arr_common_prdt_dvs as $arr_prdt_dvs) {
            if (!empty($arr_prdt_dvs)) {
                $prefix = $arr_prdt_dvs . '_';

                $common_cate_sortcode = $fb[$prefix . "cate_sortcode"];
                $common_sell_price = $fb[$prefix . "sell_price"];
                $common_sale_price = $fb[$prefix . "sale_price"];
                $common_count        = $fb[$prefix . "count"];
                if($common_count == "") {
                    $common_count = "1";
                }

                $this->connectionPool = new ConnectionPool();
                $this->conn = $this->connectionPool->getPooledConnection();
                $this->dao = new ProductCommonDAO();
                $cate_info = $this->dao->selectCateInfo($this->conn, $common_cate_sortcode);
                $flattyp_yn   = $cate_info["flattyp_yn"];
                $tmpt_dvs     = $cate_info["tmpt_dvs"];
                $amt_unit_dvs = $cate_info["amt_unit"];
                unset($cate_info);
                $this->order_common_param['tmpt_dvs'] = $tmpt_dvs;
                $this->order_common_param['flattyp_yn'] = $flattyp_yn;
                $this->order_common_param['amt_unit_dvs'] = $amt_unit_dvs;
                $this->order_common_param["cate_sortcode"] = $common_cate_sortcode;

                $this->order_common_param["mono_yn"]       = 'N';
                $this->order_common_param["count"]         = $common_count;

                if (empty($fb[$prefix . "sheet_count"]) === true) {
                    $amt = doubleval($fb[$prefix . "amt"]);
                } else {
                    $amt = doubleval($fb[$prefix . "sheet_count"]);
                }

                $this->order_common_param["page_cnt"] = $amt;
                // 주문_번호
                $order_num = $frontUtil->makeOrderNum($this->conn, $this->dao,
                    $common_cate_sortcode);
                $this->order_common_param["order_num"] = $order_num;

                // 공통 후공정 가격(제본비 등)
                $common_after_price = $fb[$prefix . "after_price"];

                // 책자 제본 정보
                $binding_mpcode = $fb[$prefix . "binding_val"];

                $cate_name         = $fb[$prefix . "cate_name"]; //1
                $size_name         = $fb[$prefix . "size_name"]; // 3
                $paper_name        = $fb[$prefix . "paper_name"]; // 2
                $bef_tmpt_name     = $fb[$prefix . "bef_tmpt_name"];
                $aft_tmpt_name     = $fb[$prefix . "aft_tmpt_name"];
                $bef_add_tmpt_name = $fb[$prefix . "bef_add_tmpt_name"];
                $aft_add_tmpt_name = $fb[$prefix . "aft_add_tmpt_name"];

                $tmpt_str = '';
                if ($tmpt_dvs === '0') {
                    $tmpt_str = $bef_tmpt_name;
                } else {
                    $tmpt_str = "전면 : " . $bef_tmpt_name;
                    if (!empty($bef_add_tmpt_name)) {
                        $tmpt_str .= ", 전면추가 : " . $bef_add_tmpt_name;
                    }
                    if (!empty($aft_tmpt_name)) {
                        $tmpt_str .= ", 후면 : " . $aft_tmpt_name;
                    }
                    if (!empty($aft_add_tmpt_name)) {
                        $tmpt_str .= ", 후면추가 : " . $aft_add_tmpt_name;
                    }
                }

                $param["util"]       = $fb;
                $param["prefix"]       = $prefix;
                $expec_weight = $frontUtil->calcExpectWeight($this->conn, $this->dao, $param);
                $this->order_common_param["expec_weight"] = $expec_weight;
            }
        }

        //$this->order_common_param["order_detail"] = sprintf("%s / %s / %s / %s", $cate_name
        //    , $paper_name
        //    , $size_name
        //    , $tmpt_str);
        $this->order_common_param["opt_use_yn"] = 'Y';
        if (empty($fb["opt_add"]) === true) {
            $this->order_common_param["opt_use_yn"] = 'N';
        }

        $this->order_common_param["owncompany_img_num"] = '';
        if (empty($fb["owncompany_img_num"]) === true) {
            $this->order_common_param["owncompany_img_num"] = '';
        } else {
            $this->order_common_param["owncompany_img_num"] = $fb["owncompany_img_num"];
        }

        $this->order_common_param["dlvr_produce_dvs"] = "지방판";

        $this->order_common_param["sell_price"] = $price["sell_price"];
        $this->order_common_param["pay_price"] = $price["sell_price"];
        $this->order_common_param["sell_price"] = intval(str_replace(',', '', $fb["ad_sale_price"]));
        $this->order_common_param["pay_price"] = intval(str_replace(',', '', $fb["ad_sell_price"]));
        $this->order_common_param["grade_sale_price"] = 0;
        $this->order_common_param["use_point_price"] = 0;
        $this->order_common_param["event_price"] = 0;
        $this->order_common_param["member_sale_price"] = 0;
        $this->order_common_param["receipt_dvs"] = "Manual";
        $this->order_common_param["bun_group"] = "to_1";
        $this->order_common_param["bun_yn"] = "N";
        $this->order_common_param["add_after_price"]  = $price["add_after_price"];
        if($this->order_common_param["add_after_price"] == "") {
            $this->order_common_param["add_after_price"] = 0;
        }
        $this->order_common_param["commerce_dvs"]  = "일반"; // 일반, 상업
        $this->order_common_param["dlvr_dvs"]  = "namecard";
    }

    function makeOrderDetailInsertParam($fb, $session) {
        //$this->order_detail_param = array();
        $this->order_detail_param["order_common_seqno"] = $this->order_common_param["order_common_seqno"];
        $this->order_detail_param["cate_sortcode"] = $this->order_common_param["cate_sortcode"];
        $this->order_detail_param["order_detail_dvs_num"] = "S" . $this->order_common_param["order_num"] . "01";
        $this->order_detail_param["state"] = "1220";
        $this->order_detail_param["typ"] = "표지";
        $this->order_detail_param["sell_price"] = $this->order_common_param["sell_price"];
        $this->order_detail_param["grade_sale_price"] = 0;
        $this->order_detail_param["member_sale_price"] = 0;
        $this->order_detail_param["use_point_price"] = 0;
        $this->order_detail_param["pay_price"] = 0;
        $this->order_detail_param["del_yn"] = "N";
        $this->order_detail_param["amt"] = $this->order_common_param["amt"];
        $this->order_detail_param["count"] = $this->order_common_param["count"];

        $common_prdt_dvs = $fb["common_prdt_dvs"];
        $prefix = $common_prdt_dvs . "_";
        $this->order_detail_param["work_size_wid"] = $fb[$prefix . "work_wid_size"];
        $this->order_detail_param["work_size_vert"] = $fb[$prefix . "work_vert_size"];
        $this->order_detail_param["cut_size_wid"] = $fb[$prefix . "cut_wid_size"];
        $this->order_detail_param["cut_size_vert"] = $fb[$prefix . "cut_vert_size"];
        $this->order_detail_param["receipt_size_wid"] = $this->order_detail_param["work_wid_size"];
        $this->order_detail_param["receipt_size_vert"] = $this->order_detail_param["work_vert_size"];
        $this->order_detail_param["tomson_size_wid"] = $fb[$prefix . "tomson_size_wid"];
        $this->order_detail_param["tomson_size_vert"] = $fb[$prefix . "tomson_size_vert"];
        $this->order_detail_param["amt_unit_dvs"] = $fb[$prefix . "amt_unit"];
        $this->order_detail_param["stan_name"] = $fb[$prefix . "stan_name"];
        if (!empty($common_prdt_dvs)) {
            $prefix = $common_prdt_dvs . '_';
            $this->order_detail_param["cate_paper_mpcode"] = $fb[$prefix . "paper"];
            $this->order_detail_param["cate_paper_tot_mpcode"] = $fb[$prefix . "paper"];
            if (!empty($fb[$prefix . "paper_list"])) {
                $this->order_detail_param["cate_paper_tot_mpcode"] = $fb[$prefix . "paper_list"];
            }

            $this->order_detail_param["cate_bef_print_mpcode"] = intval($fb[$prefix . "bef_tmpt"]);
            $this->order_detail_param["cate_aft_print_mpcode"] = intval($fb[$prefix . "aft_tmpt"]);
            $this->order_detail_param["cate_bef_add_print_mpcode"] = intval($fb[$prefix . "bef_add_tmpt"]);
            $this->order_detail_param["cate_aft_add_print_mpcode"] = intval($fb[$prefix . "aft_add_tmpt"]);
            $this->order_detail_param["cate_output_mpcode"] = $fb[$prefix . "size"];

            $this->order_detail_param["order_detail"] = $this->order_common_param["order_detail"];
            $this->order_detail_param["mono_yn"] = "N";

            $size_dvs = $fb[$prefix . "size_dvs"];
            if ($size_dvs === "manu") {
                $stan_name = "비규격";
            } else {
                $stan_name = $fb[$prefix . "size_name"];
            }

            $this->order_detail_param["stan_name"] = $stan_name;
            $this->order_detail_param["print_purp_dvs"] = $fb[$prefix . "print_purp"];
            $tot_tmpt = 0;
            // 단면/양면 구분값
            $side_dvs = null;
            if (empty($fb[$prefix . "tot_tmpt"]) === true) {
                $tot_tmpt = $fb[$prefix . "tot_tmpt"];
            }

            $param = array();
            $param["typ"]        = "표지";
            $param["tmpt_dvs"]   = $this->order_detail_param["tmpt_dvs"];
            $param["flattyp_yn"] = $this->order_detail_param["flattyp_yn"];
            $param["util"]       = new FrontCommonUtil();
            $param["fb"]       = $fb;
            $tmpt_name = $this->makePrintTmptName($this->conn,
                $this->dao,
                $param,
                $tot_tmpt,
                $side_dvs);
            $this->order_detail_param["expec_weight"] = $this->order_common_param["expec_weight"];
            $this->order_detail_param["print_tmpt_name"] = $tmpt_name;
            $this->order_detail_param["tot_tmpt"] = $tot_tmpt;
            $this->order_detail_param["page_amt"] = $fb[$prefix . "page"];
            if (!$this->order_detail_param["page_amt"]) {
                $this->order_detail_param["page_amt"] = 2;
            }
            $this->order_detail_param["amt_unit_dvs"] = $this->order_common_param["amt_unit_dvs"];

            $this->order_detail_param["side_dvs"] = $side_dvs;
            $this->order_detail_param["tomson_yn"] = "N";
            $this->order_detail_param["typset_way"] = "CYPRESS";
        }
    }
}

?>