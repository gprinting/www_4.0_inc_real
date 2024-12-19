<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-09-04
 * Time: 오후 6:14
 * 배송관리 어플리케이션용 DAO
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/common_lib/cypress_file.inc");
include_once(INC_PATH . "/classes/dprinting/PriceCalculator/Common/DPrintingFactory.php");

$curDirectory = dirname(__FILE__);
define("LOGPATH", $curDirectory . "/logs/" . date("Y_m_d"));

class DeliveryAppDAO extends NimdaCommonDAO
{
    function __construct()
    {
    }

    function numberFormatClean($iNumber, $iDecimals = 2)
    {
        $sNumber = number_format($iNumber, $iDecimals);
        $sNumber = rtrim($sNumber, 0);
        $sNumber= rtrim($sNumber, '.');

        return $sNumber;
    }

    function selectAfterString($conn, $order_common_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT * ";
        $query .= "\n   FROM order_after_history ";
        $query .= "\n   WHERE order_common_seqno = '%s' AND basic_yn = 'N' ";

        $query = sprintf($query, $order_common_seqno);

        $rs = $conn->Execute($query);
        $str = "";
        while($rs && !$rs->EOF) {
            $str .= ", " . $rs->fields['after_name'];
            $rs->MoveNext();
        }

        if($str != "") {
            $str = substr($str, 2);
        }

        return $str;
    }

    public function selectDlvrInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT * FROM order_dlvr ";
        $query .= "\n   WHERE bun_dlvr_order_num = %s AND bun_group = %s ";
        $query .= "\n   GROUP BY tsrs_dvs ";

        $query = sprintf($query, $param['bun_dlvr_order_num'], $param['bun_group']);

        $rs = $conn->Execute($query);
        $dlvr_info = array();
        while($rs && !$rs->EOF) {
            if($rs->fields['tsrs_dvs'] == "수신") {
                $dlvr_info['ReceiverName'] = $rs->fields['name'];
                if($rs->fields['tel_num'] != "--" || $rs->fields['tel_num'] != "") {
                    $phone = explode('-', $rs->fields['tel_num']);
                    $dlvr_info['ReceiverPhone1'] = $phone[0];
                    $dlvr_info['ReceiverPhone2'] = $phone[1];
                    $dlvr_info['ReceiverPhone3'] = $phone[2];
                } else {
                    $dlvr_info['ReceiverPhone1'] = "";
                    $dlvr_info['ReceiverPhone2'] = "";
                    $dlvr_info['ReceiverPhone3'] = "";
                }
                if($rs->fields['cell_num'] != "--" || $rs->fields['cell_num'] != "") {
                    $cell = explode('-', $rs->fields['cell_num']);
                    $dlvr_info['ReceiverCell1'] = $cell[0];
                    $dlvr_info['ReceiverCell2'] = $cell[1];
                    $dlvr_info['ReceiverCell3'] = $cell[2];
                } else {
                    $dlvr_info['ReceiverCell1'] = "";
                    $dlvr_info['ReceiverCell2'] = "";
                    $dlvr_info['ReceiverCell3'] = "";
                }
                $dlvr_info['ReceiverAddress1'] = $rs->fields['addr'];
                $dlvr_info['ReceiverAddress2'] = $rs->fields['addr_detail'];
                $dlvr_info['ReceiverZipcode'] = $rs->fields['zipcode'];
                $SelectedDeliveryWay = $rs->fields['dlvr_way'];
                if($SelectedDeliveryWay == "01")
                {
                    $SelectedDeliveryWay = "택배";
                }
                else if ($SelectedDeliveryWay == "02")
                {
                    $SelectedDeliveryWay = "직배";
                }
                else if ($SelectedDeliveryWay == "03")
                {
                    $SelectedDeliveryWay = "화물";
                }
                else if ($SelectedDeliveryWay == "04")
                {
                    $SelectedDeliveryWay = "퀵";
                }
                else if ($SelectedDeliveryWay == "05")
                {
                    $SelectedDeliveryWay = "인방";
                }
                else if ($SelectedDeliveryWay == "06")
                {
                    $SelectedDeliveryWay = "필방";
                }

                $dlvr_info['SelectedDeliveryWay'] = $SelectedDeliveryWay;
                $dlvr_info['DlvrPrice'] = $rs->fields['dlvr_price'];
                $dlvr_info['PayWay'] = $rs->fields['dlvr_sum_way'];

            } else if($rs->fields['tsrs_dvs'] == "송신") {
                $dlvr_info['SenderName'] = $rs->fields['name'];
                if($rs->fields['tel_num'] != "--" || $rs->fields['tel_num'] != "") {
                    $phone = explode('-', $rs->fields['tel_num']);
                    $dlvr_info['SenderPhone1'] = $phone[0];
                    $dlvr_info['SenderPhone2'] = $phone[1];
                    $dlvr_info['SenderPhone3'] = $phone[2];
                } else {
                    $dlvr_info['SenderPhone1'] = "";
                    $dlvr_info['SenderPhone2'] = "";
                    $dlvr_info['SenderPhone3'] = "";
                }
                if($rs->fields['cell_num'] != "--" || $rs->fields['cell_num'] != "") {
                    $cell = explode('-', $rs->fields['cell_num']);
                    $dlvr_info['SenderCell1'] = $cell[0];
                    $dlvr_info['SenderCell2'] = $cell[1];
                    $dlvr_info['SenderCell3'] = $cell[2];
                } else {
                    $dlvr_info['SenderCell1'] = "";
                    $dlvr_info['SenderCell2'] = "";
                    $dlvr_info['SenderCell3'] = "";
                }
                $dlvr_info['SenderAddress1'] = $rs->fields['addr'];
                $dlvr_info['SenderAddress2'] = $rs->fields['addr_detail'];
                $dlvr_info['SenderZipcode']  = $rs->fields['zipcode'];
            }
            $rs->MoveNext();
        }

        return $dlvr_info;
    }

    public function selectDeliveryListByBtn($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    select distinct(E.bun_dlvr_order_num), E.bun_group, E.name, E.tel_num, F.member_name, ";
        $query .= "\n   E.cell_num, E.addr, E.addr_detail, F.member_seqno, E.invo_cpn, E.lump_count, E.dlvr_way, E.dlvr_sum_way, E.bun_group  ";
        $query .= "\n   FROM order_detail_count_file AS A   ";
        $query .= "\n   INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state ";
        $query .= "\n   INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno  ";
        $query .= "\n   INNER JOIN order_dlvr AS E ON E.order_common_seqno = D.order_common_seqno and E.tsrs_dvs = '수신' ";
        $query .= "\n   INNER JOIN member AS F ON D.member_seqno = F.member_seqno ";
        $query .= "\n   WHERE 1 = 1 ";

        if($param['option'] == "CntAll") {
            $query .= "\n   AND A.state BETWEEN '2120' AND '3320' ";
        } else if($param['option'] == "CntDelay") {
            $query .= "\n   AND A.state BETWEEN '3220' and '3320' ";
            $query .= "\n   AND B.change_date < DATE_ADD(NOW(), INTERVAL -6 HOUR) ";
        } else if($param['option'] == "CntProduceYesterday") {
            $query .= "\n   AND A.state = '2120' AND B.change_date >= DATE(DATE_ADD(NOW(), INTERVAL -1 DAY)) AND B.change_date < DATE(NOW()) ";
        } else if($param['option'] == "CntProduceToday") {
            $query .= "\n   AND A.state = '2120' AND B.change_date > DATE(NOW()) ";
        } else if($param['option'] == "CntWillInwarehouse") {
            $query .= "\n   AND A.state = '3120' ";
        } else if($param['option'] == "CntParcel") {
            $query .= "\n   AND A.state BETWEEN '3220' AND '3330' ";
            $query .= "\n   AND E.dlvr_way = '01' ";
        }

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND D.dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND D.dlvr_dvs = 'le' \n";
        }

        $query .= "\n  ORDER BY E.addr, E.addr_detail, E.name ";

        $rs = $conn->Execute($query);

        $json = array();
        $json['customers'] = array();
        while($rs && !$rs->EOF) {
            $part_query = "\n SELECT ";
            $part_query .= "\n   A.order_common_seqno, B.order_detail, B.amt ,B.amt_unit_dvs, B.count, B.order_regi_date ";
            $part_query .= "\n  , B.title, B.expec_weight, B.order_state, C.cut_size_wid, C.cut_size_vert, B.receipt_finish_date ";
            $part_query .= "\n  , D.name, D.color, D.basisweight ";
            $part_query .= "\n  FROM order_dlvr AS A ";
            $part_query .= "\n  INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
            $part_query .= "\n  INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
            $part_query .= "\n  INNER JOIN cate_paper AS D ON C.cate_paper_mpcode = D.mpcode ";
            $part_query .= "\n  WHERE A.bun_dlvr_order_num = '" . $rs->fields['bun_dlvr_order_num'] . "' and A.tsrs_dvs = '수신' ";
            $part_query .= "\n  AND A.bun_group = '" . $rs->fields['bun_group'] . "' ";

            $part_rs = $conn->Execute($part_query);
            $customer = array();
            $customer['memseq'] = $rs->fields['member_seqno'];
            $customer['Company'] = $rs->fields['member_name'];
            $customer['name'] = $rs->fields['name'];
            $customer['telnum'] = $rs->fields['tel_num'];
            $customer['celnum'] = $rs->fields['cell_num'];
            $customer['Address'] = $rs->fields['addr'] . " " . $rs->fields['addr_detail'];
            $customer['BunNum'] =  $rs->fields['bun_dlvr_order_num'];
            $customer['bungroup'] =  $rs->fields['bun_group'];
            $customer['DeliveryWay'] =  $rs->fields['invo_cpn'];
            $customer['DeliveryKind'] = $rs->fields['dlvr_way'];
            $customer['DeliveryPayKind'] = $rs->fields['dlvr_sum_way'];
            $customer['lump'] = $rs->fields['lump_count'];
            $customer['Weight'] = 0;
            $customer['Products'] = array();
            while($part_rs && !$part_rs->EOF) {
                $product = array();
                $customer['Weight'] += $part_rs->fields['expec_weight'];
                $product['orderseq'] = $part_rs->fields['order_common_seqno'];
                $product['paper'] = $part_rs->fields['name'] . " " . $part_rs->fields['color'] . " " . $part_rs->fields['basisweight'] ;
                $product['size'] = $part_rs->fields['cut_size_wid'] . " X " . $part_rs->fields['cut_size_vert'];
                $product['amt'] = $this->numberFormatClean($part_rs->fields['amt']);
                $product['amtunit'] = $part_rs->fields['amt_unit_dvs'];
                $product['count'] = $part_rs->fields['count'];
                $product['title'] = $part_rs->fields['title'];
                $product['state'] = $part_rs->fields['order_state'];
                $product['OrderDate'] = $part_rs->fields['order_regi_date'];
                $product['ReceiptDate'] = $part_rs->fields['receipt_finish_date'];
                $product['after'] = $this->selectAfterString($conn, $part_rs->fields['order_common_seqno']);
                $product['imagepath'] = $this->selectImages($conn, $part_rs->fields['order_common_seqno']);
                array_push($customer['Products'], $product);
                $part_rs->MoveNext();
            }

            $customer['Weight'] = round($customer['Weight'],2);
            array_push($json['customers'], $customer);

            $rs->MoveNext();
        }

        return $json;
    }

    public function selectDeliveryList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $searchdate = $param['searchdate'];

        $title = $param['title'];
        $membername = $param['membername'];

        $isincludedafterprocess = $param['isincludedafterprocess'];
        $Istodaydelivery = $param['Istodaydelivery'];
        $ifinwarehouse = $param['ifinwarehouse'];
        $category = $param['category'];
        $categorydetail = $param['categorydetail'];
        $onlynotprinted = $param['onlynotprinted'];
        $deliveryway_depth1 = $param['deliveryway_depth1'];
        $deliveryway_depth2 = $param['deliveryway_depth2'];

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT distinct(C.bun_dlvr_order_num), C.name, C.tel_num, B.member_name ";
        $query .= "\n    , C.cell_num, C.addr, C.addr_detail, B.member_seqno, C.invo_cpn ";
        $query .= "\n    , C.lump_count, C.dlvr_way, C.dlvr_sum_way, C.bun_group ";
        $query .= "\n   FROM order_common AS A ";
        $query .= "\n   INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS C ON A.order_common_seqno = C.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n   INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno ";

        if($Istodaydelivery == "Y") {
            $query .= "\n   INNER JOIN order_opt_history AS E ON E.order_common_seqno = A.order_common_seqno AND E.opt_name = '당일판'";
        }
        $query .= "\n  WHERE 1 = 1 ";
        $query .= "\n  AND (A.title like '%%%s%%' OR B.member_name like '%%%s%%') ";

        if($searchdate == "접수일자")
            $query .= "\n  AND A.receipt_finish_date BETWEEN %s AND %s ";
        else
            $query .= "\n  AND A.order_regi_date BETWEEN %s AND %s ";

        if($deliveryway_depth1 != "") {
            $query .= "\n  AND C.dlvr_way = " . $param['deliveryway_depth1'];
            if($deliveryway_depth1 == '01' && $deliveryway_depth2 == "미출력만") {
                $query .= "\n  AND (C.invo_num is null OR C.invo_num = '') ";
            }

            if($deliveryway_depth1 == '02' && $deliveryway_depth2 != "") {
                $query .= "\n  AND (C.invo_cpn = '".$deliveryway_depth2."') ";
            }
        }

        if($isincludedafterprocess != "")
            $query .= "\n  AND D.after_use_yn = " . $param['isincludedafterprocess'];

        if($ifinwarehouse == "미입고") {
            $query .= "\n   AND A.order_state <= 3120 ";
        } else if($ifinwarehouse == "입고완료") {
            $query .= "\n   AND A.order_state > 3120 ";
        }



        if($category == "명함류") {
            $query .= "\n   AND A.dlvr_dvs = 'namecard' ";
            if($categorydetail == "명함|스티커") {
                $query .= "\n  AND A.cate_sortcode != '001005002' ";
            } else if($categorydetail == "복권") {
                $query .= "\n  AND A.cate_sortcode = '001005002' ";
            }
        } else if($category == "전단류") {
            $query .= "\n   AND A.dlvr_dvs = 'leaflet' ";
            if($categorydetail == "전단") {
                $query .= "\n  AND A.order_num like '%%BL%%' ";
            } else if($categorydetail == "봉투") {
                $query .= "\n  AND A.order_num like '%%EV%%' ";
            } else if($categorydetail == "마스터") {
                $query .= "\n  AND A.order_num like '%%MT%%' ";
            }

        }

        $query .= "\n  ORDER BY C.addr, C.addr_detail, C.name ";


        $query = sprintf($query
            ,$membername
            ,$membername
            ,$param['startdate']
            ,$param['enddate']);



        $rs = $conn->Execute($query);

        $json = array();
        $json['customers'] = array();
        $i = 0;
        while($rs && !$rs->EOF) {
            $i++;
            if($i > 100) break;
            $part_query = "\n SELECT ";
            $part_query .= "\n   A.order_common_seqno, B.order_detail, B.amt ,B.amt_unit_dvs, B.count, B.order_regi_date ";
            $part_query .= "\n  , B.title, B.expec_weight, B.order_state, C.cut_size_wid, C.cut_size_vert, B.receipt_finish_date ";
            $part_query .= "\n  , D.name, D.color, D.basisweight ";
            $part_query .= "\n  FROM order_dlvr AS A ";
            $part_query .= "\n  INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
            $part_query .= "\n  INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
            $part_query .= "\n  INNER JOIN cate_paper AS D ON C.cate_paper_mpcode = D.mpcode ";
            $part_query .= "\n  WHERE A.bun_dlvr_order_num = '" . $rs->fields['bun_dlvr_order_num'] . "' and A.tsrs_dvs = '수신' ";
            $part_query .= "\n  AND A.bun_group = '" . $rs->fields['bun_group'] . "' ";

            $part_rs = $conn->Execute($part_query);
            $customer = array();
            $customer['memseq'] = $rs->fields['member_seqno'];
            $customer['Company'] = $rs->fields['member_name'];
            $customer['name'] = $rs->fields['name'];
            $customer['telnum'] = $rs->fields['tel_num'];
            $customer['celnum'] = $rs->fields['cell_num'];
            $customer['Address'] = $rs->fields['addr'] . " " . $rs->fields['addr_detail'];
            $customer['BunNum'] =  $rs->fields['bun_dlvr_order_num'];
            $customer['bungroup'] =  $rs->fields['bun_group'];
            $customer['DeliveryWay'] =  $rs->fields['invo_cpn'];
            $customer['DeliveryKind'] = $rs->fields['dlvr_way'];
            $customer['DeliveryPayKind'] = $rs->fields['dlvr_sum_way'];
            $customer['lump'] = $rs->fields['lump_count'];
            $customer['Weight'] = 0;
            $customer['Products'] = array();
            while($part_rs && !$part_rs->EOF) {
                $product = array();
                $customer['Weight'] += $part_rs->fields['expec_weight'];
                $product['orderseq'] = $part_rs->fields['order_common_seqno'];
                $product['paper'] = $part_rs->fields['name'] . " " . $part_rs->fields['color'] . " " . $part_rs->fields['basisweight'] ;
                $product['size'] = $part_rs->fields['cut_size_wid'] . " X " . $part_rs->fields['cut_size_vert'];
                $product['amt'] = $this->numberFormatClean($part_rs->fields['amt']);
                $product['amtunit'] = $part_rs->fields['amt_unit_dvs'];
                $product['count'] = $part_rs->fields['count'];
                $product['title'] = $part_rs->fields['title'];
                $product['state'] = $part_rs->fields['order_state'];
                $product['OrderDate'] = $part_rs->fields['order_regi_date'];
                $product['ReceiptDate'] = $part_rs->fields['receipt_finish_date'];
                $product['after'] = $this->selectAfterString($conn, $part_rs->fields['order_common_seqno']);
                $product['imagepath'] = $this->selectImages($conn, $part_rs->fields['order_common_seqno']);
                array_push($customer['Products'], $product);
                $part_rs->MoveNext();
            }

            $customer['Weight'] = round($customer['Weight'],2);
            array_push($json['customers'], $customer);

            $rs->MoveNext();
        }

        return $json;
    }

    function selectVisitInvoiceInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);


    }

    function selectImages($conn, $order_common_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n   SELECT file_path, save_file_name  ";
        $query .= "\n  FROM order_detail AS A ";
        $query .= "\n  INNER JOIN order_detail_count_file AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n  WHERE A.order_common_seqno = %s  ";

        $query = sprintf($query, $order_common_seqno);
        $rs = $conn->Execute($query);

        $images = array();
        while($rs && !$rs->EOF) {
            $path = $rs->fields['file_path'] . "/" . $rs->fields['save_file_name'];
            array_push($images, $path);
            $rs->MoveNext();
        }

        return $images;
    }

    function selectCartOrderList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("order_common_seqno" => true);
        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.order_regi_date";
        $query .= "\n        ,A.order_common_seqno";
        $query .= "\n        ,A.order_num";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.order_detail";
        $query .= "\n        ,A.sell_price";
        $query .= "\n        ,A.grade_sale_price";
        $query .= "\n        ,A.member_sale_price";
        $query .= "\n        ,A.add_after_price";
        $query .= "\n        ,A.add_opt_price";
        $query .= "\n        ,A.event_price";
        $query .= "\n        ,A.expec_weight";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.count";

        $query .= "\n   FROM  order_common AS A";

        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $query .= "\n    AND  A.order_common_seqno IN (";
            $query .= $param["order_common_seqno"];
            $query .= ')';
        }

        if ($this->blankParameterCheck($param,"member_seqno")) {
            $query .= "\n   AND A.member_seqno = " . $param['member_seqno'];
        }

        $rs = $conn->Execute($query);

        return $rs;
    }

    function selectIslandParcelCost($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $first_zipcode = substr($param['zipcode'], 1, 1);

        $query  = "\n SELECT  distinct price ";
        $query  .= "\n FROM  CJparcel_islands ";
        $query .= "\n  WHERE  new_zipcode = '%s'";

        $query  = sprintf($query, $param['zipcode']);

        return $conn->Execute($query);
    }

    function selectInfoForParcel($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $bun_dlvr_order_num = $param['bun_dlvr_order_num'];
        $bun_group = $param['bun_group'];

        $param = $this->parameterArrayEscape($conn, $param);

        $lump_count = 1;
        $dlvr_price = 0;

        $result = array();
        $results = array();

        $query = "\n    SELECT ";
        $query .= "\n   A.name RCVR_NM";
        $query .= "\n    , A.tel_num RCVR_TEL ";
        $query .= "\n    , A.cell_num RCVR_CELL ";
        $query .= "\n    , A.zipcode RCVR_ZIP_NO ";
        $query .= "\n    , A.addr RCVR_ADDR ";
        $query .= "\n    , A.addr_detail RCVR_DETAIL_ADDR ";
        $query .= "\n    , C.member_name ORDRR_NM ";
        $query .= "\n    , C.tel_num ORDRR_TEL ";
        $query .= "\n    , C.cell_num ORDRR_CELL ";
        $query .= "\n    , C.zipcode ORDRR_ZIP_NO ";
        $query .= "\n    , C.addr ORDRR_ADDR ";
        $query .= "\n    , C.addr_detail ORDRR_DETAIL_ADDR ";
        $query .= "\n    , SUM(B.pay_price) ARTICLE_AMT ";
        $query .= "\n    , SUM(B.pay_price) GDS_AMT ";
        $query .= "\n    , GROUP_CONCAT(CONCAT(B.title, \"(\", B.order_detail, \")\") SEPARATOR '\n') GDS_NM  ";
        $query .= "\n   , A.lump_count ";
        $query .= "\n   , A.dlvr_price ";
        $query .= "\n   , A.dlvr_sum_way ";
        $query .= "\n   , GROUP_CONCAT(CONCAT(B.amt, B.amt_unit_dvs,\"X\", B.count, \"건\") SEPARATOR ', ') AMT";
        $query .= "\n    FROM order_dlvr AS A ";
        $query .= "\n    LEFT JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n    INNER JOIN member AS C ON B.member_seqno = C.member_seqno ";
        $query .= "\n    WHERE A.bun_dlvr_order_num = %s and A.bun_group = %s and A.tsrs_dvs = '수신' ";
        $query .= "\n    group by A.bun_dlvr_order_num, A.bun_group, A.tsrs_dvs ";

        $query  = sprintf($query
            ,$param['bun_dlvr_order_num']
            ,$param['bun_group']);

        $rs = $conn->Execute($query);

        while($rs && !$rs->EOF) {
            $result['RCVR_NM'] = $rs->fields['RCVR_NM'];

            $RCVR_TEL = explode('-', $rs->fields['RCVR_TEL']);
            $result['RCVR_TEL_NO1'] = $RCVR_TEL[0] ==  NULL ? "" : $RCVR_TEL[0];
            $result['RCVR_TEL_NO2'] = $RCVR_TEL[1] ==  NULL ? "" : $RCVR_TEL[1];
            $result['RCVR_TEL_NO3'] = $RCVR_TEL[2] ==  NULL ? "" : $RCVR_TEL[2];

            $RCVR_CELL = explode('-',$rs->fields['RCVR_CELL']);
            $result['RCVR_CELL_NO1'] = $RCVR_CELL[0]== NULL ? "" : $RCVR_CELL[0];
            $result['RCVR_CELL_NO2'] = $RCVR_CELL[1]== NULL ? "" : $RCVR_CELL[1];
            $result['RCVR_CELL_NO3'] = $RCVR_CELL[2]== NULL ? "" : $RCVR_CELL[2];

            $result['RCVR_ZIP_NO'] = $rs->fields['RCVR_ZIP_NO'];
            $result['RCVR_ADDR'] = $rs->fields['RCVR_ADDR'];
            $result['RCVR_DETAIL_ADDR'] = $rs->fields['RCVR_DETAIL_ADDR'];
            $result['ORDRR_NM'] = $rs->fields['ORDRR_NM'];

            $ORDRR_TEL = explode('-', $rs->fields['ORDRR_TEL']);
            $result['ORDRR_TEL_NO1'] = $ORDRR_TEL[0] == NULL ? "" : $ORDRR_TEL[0];
            $result['ORDRR_TEL_NO2'] = $ORDRR_TEL[1] == NULL ? "" : $ORDRR_TEL[1];
            $result['ORDRR_TEL_NO3'] = $ORDRR_TEL[2]== NULL ? "" : $ORDRR_TEL[2];

            $ORDRR_CELL = explode('-', $rs->fields['ORDRR_CELL']);
            $result['ORDRR_CELL_NO1'] = $ORDRR_CELL[0]== NULL ? "" : $ORDRR_CELL[0];
            $result['ORDRR_CELL_NO2'] = $ORDRR_CELL[1]== NULL ? "" : $ORDRR_CELL[1];
            $result['ORDRR_CELL_NO3'] = $ORDRR_CELL[2]== NULL ? "" : $ORDRR_CELL[2];

            $result['ORDRR_ZIP_NO'] = $rs->fields['ORDRR_ZIP_NO'];
            $result['ORDRR_ADDR'] = $rs->fields['ORDRR_ADDR'];
            $result['ORDRR_DETAIL_ADDR'] = $rs->fields['ORDRR_DETAIL_ADDR'];
            $result['ARTICLE_AMT'] = $rs->fields['ARTICLE_AMT'];
            $result['GDS_AMT'] = $rs->fields['GDS_AMT'];
            $result['GDS_NM'] = $rs->fields['GDS_NM'];
            $result['AMT'] = $rs->fields['AMT'];
            $rs->fields['dlvr_sum_way'] == "01" ? ($result['FRT_DV_CD'] = "03") : ($result['FRT_DV_CD'] = "02" );

            $lump_count = $rs->fields['lump_count'];
            $dlvr_price = $rs->fields['dlvr_price'];


            $rs->MoveNext();
        }

        $query = "\n    SELECT ";
        $query .= "\n   A.name SENDR_NM ";
        $query .= "\n   , A.tel_num SENDR_TEL ";
        $query .= "\n   , A.cell_num SENDR_CELL ";
        $query .= "\n   , A.zipcode SENDR_ZIP_NO ";
        $query .= "\n   , A.addr SENDR_ADDR ";
        $query .= "\n   , A.addr_detail SENDR_DETAIL_ADDR ";
        $query .= "\n   FROM order_dlvr AS A ";
        $query .= "\n   INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   WHERE A.bun_dlvr_order_num = %s and A.bun_group = %s and A.tsrs_dvs = '송신' ";
        $query .= "\n   group by A.bun_dlvr_order_num, A.bun_group, A.tsrs_dvs ";

        $query  = sprintf($query
            ,$param['bun_dlvr_order_num']
            ,$param['bun_group']);

        $rs = $conn->Execute($query);

        while($rs && !$rs->EOF) {
            $result['SENDR_NM'] = $rs->fields['SENDR_NM'];
            $SENDR_TEL = explode('-',$rs->fields['SENDR_TEL']);
            $result['SENDR_TEL_NO1'] = $SENDR_TEL[0] == NULL? "" : $SENDR_TEL[0];
            $result['SENDR_TEL_NO2'] = $SENDR_TEL[1] == NULL? "" : $SENDR_TEL[1];
            $result['SENDR_TEL_NO3'] = $SENDR_TEL[2] == NULL? "" : $SENDR_TEL[2];

            $SENDR_CELL = explode('-',$rs->fields['SENDR_CELL']);
            $result['SENDR_CELL_NO1'] = $SENDR_CELL[0] == NULL? "" : $SENDR_CELL[0];
            $result['SENDR_CELL_NO2'] = $SENDR_CELL[1] == NULL? "" : $SENDR_CELL[1];
            $result['SENDR_CELL_NO3'] = $SENDR_CELL[2] == NULL? "" : $SENDR_CELL[2];

            $result['SENDR_ZIP_NO'] = $rs->fields['SENDR_ZIP_NO'];
            $result['SENDR_ADDR'] = $rs->fields['SENDR_ADDR'];
            $result['SENDR_DETAIL_ADDR'] = $rs->fields['SENDR_DETAIL_ADDR'];
            $rs->MoveNext();
        }

        $dlvr_price /= $lump_count;
        for($i = 1 ; $i <= $lump_count ; $i++) {
            $query = "\n SELECT invo_num FROM invo_num WHERE state = 'Y' order by invo_num_seqno limit 1 ";
            $rs = $conn->Execute($query);

            while($rs && !$rs->EOF) {
                $result['INVC_NO'] = $rs->fields['invo_num'];
                $result['dlvr_price'] = $dlvr_price;
                $update_query = "\n UPDATE invo_num SET state = 'N' WHERE invo_num = '". $result['INVC_NO'] . "'";
                $conn->Execute($update_query);
                $rs->MoveNext();
            }

            $result['RCPT_YMD'] = date("Ymd");
            $result['CUST_USE_NO'] = $bun_dlvr_order_num . "_" . $bun_group . "_" . $i;
            $result['MPCK_KEY'] = $result['RCPT_YMD'] . "_". "30139938" . "_" . $result['CUST_USE_NO'];

            array_push($results, $result);
        }
        return $results;
    }

    function UpdateBunInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $bun_seq = $param['bun_seq'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_dlvr ";
        if($bun_seq == "1") {
            $query .= "\n   SET  invo_num = %s ";
        } else {
            $query .= "\n   SET  invo_num = concat(invo_num, ',', %s) ";
        }
        $query .= "\n   WHERE bun_dlvr_order_num = %s AND bun_group = %s ";

        $query = sprintf($query,
            $param['invo_num'],
            $param['bun_dlvr_order_num'],
            $param['bun_group']);

        $conn->Execute($query);
    }

    function InitBunDlvrInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_dlvr ";
        $query .= "\n   SET invo_num = '' ";
        $query .= "\n   , bun_dlvr_order_num = '%s' ";
        $query .= "\n   WHERE order_common_seqno = %s ";

        $bun_dlvr_order_num = strval(ceil(microtime(true) * 1000.0)) . rand(0, 9);

        $query = sprintf($query,
            $bun_dlvr_order_num,
            $param['order_common_seqno']);

        $conn->Execute($query);
    }

    function UpdateBunDlvrInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_dlvr ";
        $query .= "\n   SET invo_num = '' ";
        $query .= "\n   , bun_dlvr_order_num = %s ";
        $query .= "\n   , bun_group = %s ";
        $query .= "\n   WHERE bun_dlvr_order_num = %s ";
        $query .= "\n   AND bun_group = %s ";

        $query = sprintf($query,
            $param['new_bun_dlvr_order_num'],
            $param['new_bun_group'],
            $param['bun_dlvr_order_num'],
            $param['bun_group']);

        $conn->Execute($query);
    }

    function SelectAlreadyPrintedParcels($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $keyword = $param['bun_dlvr_order_num'] . "_" . $param['bun_group'];

        $query = "\n    SELECT * ";
        $query .= "\n   FROM  CJparcel_record ";
        $query .= "\n   WHERE order_num like '%" . $keyword . "%'";

        $rs = $conn->Execute($query);

        $results = array();
        while($rs && !$rs->EOF) {
            $result = array();
            $result["boxCount"] = $rs->fields['boxCount'];
            $result["branshortnm"] = $rs->fields['branshortnm'];
            $result["calgubun"] = $rs->fields['calgubun'];
            $result["clsfaddr"] = $rs->fields['clsfaddr'];
            $result["clsfcd"] = $rs->fields['clsfcd'];
            $result["cost"] = $rs->fields['cost'];
            $result["from_address"] = $rs->fields['from_address'];
            $result["from_cellNum"] = $rs->fields['from_cellNum'];
            $result["from_name"] = $rs->fields['from_name'];
            $result["from_tellNum"] = $rs->fields['from_tellNum'];
            $result["invoiceNumber"] = $rs->fields['invoiceNumber'];
            $result["receiptDate"] = $rs->fields['receiptDate'];
            $result["subclsfcd"] = $rs->fields['subclsfcd'];
            $result["title"] = $rs->fields['title'];
            $result["to_address"] = $rs->fields['to_address'];
            $result["to_cellNum"] = $rs->fields['to_cellNum'];
            $result["to_name"] = $rs->fields['to_name'];
            $result["to_tellNum"] = $rs->fields['to_tellNum'];
            $result["vempnicknm"] = $rs->fields['vempnicknm'];
            $result["vempnm"] = $rs->fields['vempnm'];
            $result["order_num"] = $rs->fields['order_num'];
            $result["print_count"] = $rs->fields['print_count'];

            array_push($results, $result);

            $rs->MoveNext();
        }

        return $results;
    }

    function selectCntAll($conn, $param) {
        $query = " SELECT count(T.order_common_seqno) as cnt \n";
        $query .= " FROM ( \n";
        $query .= " SELECT A.order_common_seqno  \n";
        $query .= " FROM order_dlvr AS A \n";
        $query .= " INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " WHERE B.order_state BETWEEN '2120' AND '3320' \n";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND dlvr_dvs = 'le' \n";
        }

        //$query .= " AND B.receipt_finish_date like '2017-10-16%' \n";
        $query .= " GROUP BY A.bun_dlvr_order_num ,A.bun_group \n";
        $query .= " ) as T\n";
        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }



    function selectCntDelay($conn, $param) {
        $query = " select count(c.order_detail_seqno) as cnt from ( \n";
        $query .= " select A.order_detail_seqno from order_detail_count_file AS A  \n";
        $query .= "  INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state \n";
        $query .= "  INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno \n";
        $query .= "  INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno \n";
        $query .= "  WHERE A.state BETWEEN '3220' and '3320' \n";
        $query .= " AND B.change_date < DATE_ADD(NOW(), INTERVAL -6 HOUR) \n";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND D.dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND D.dlvr_dvs = 'le' \n";
        }

        $query .= " ) as c \n";

        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }

    function selectCntProduceYesterday($conn, $param) {
        $query = " select count(c.order_detail_seqno) as cnt from ( \n";
        $query .= " select A.order_detail_seqno from order_detail_count_file AS A  \n";
        $query .= "  INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state \n";
        $query .= "  INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno \n";
        $query .= "  INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno \n";
        $query .= "  WHERE  A.state = '2120' AND B.change_date >= DATE(DATE_ADD(NOW(), INTERVAL -1 DAY)) AND B.change_date < DATE(NOW()) \n";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND D.dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND D.dlvr_dvs = 'le' \n";
        }

        $query .= " ) as c \n";

        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }

    function selectProduceToday($conn, $param) {
        $query = " select count(c.order_detail_seqno) as cnt from ( \n";
        $query .= " select A.order_detail_seqno from order_detail_count_file AS A  \n";
        $query .= "  INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state \n";
        $query .= "  INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno \n";
        $query .= "  INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno \n";
        $query .= "  WHERE  A.state = '2120' AND B.change_date > DATE(NOW()) \n";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND dlvr_dvs = 'le' \n";
        }

        $query .= " ) as c \n";
        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }

    function selectCntWillInwarehouse($conn, $param) {
        $query = " select count(c.order_detail_seqno) as cnt from ( \n";
        $query .= " select A.order_detail_seqno from order_detail_count_file AS A  \n";
        $query .= "  INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state \n";
        $query .= "  INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno \n";
        $query .= "  INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno \n";
        $query .= "  WHERE  A.state = '3120'  \n";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND dlvr_dvs = 'le' \n";
        }

        $query .= " ) as c \n";
        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }

    function selectCntParcel($conn, $param) {
        $query = " select count(c.order_detail_seqno) as cnt from ( \n";
        $query .= " select A.order_detail_seqno from order_detail_count_file AS A  \n";
        $query .= "  INNER JOIN state_change_admin AS B ON A.order_detail_file_num = B.order_detail_file_num and A.state = B.aft_state \n";
        $query .= "  INNER JOIN order_detail AS C ON C.order_detail_seqno = A.order_detail_seqno \n";
        $query .= "  INNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno \n";
        $query .= "  INNER JOIN order_dlvr AS E ON E.order_common_seqno = D.order_common_seqno \n";
        $query .= "  WHERE  A.state BETWEEN '3220' AND '3330'  \n";
        $query .= " AND D.dlvr_dvs = 'na' AND E.dlvr_way = '01' \n ";

        if($param['usedplace'] == "명함출고실") {
            $query .= " AND dlvr_dvs = 'na' \n";
        } else if ($param['usedplace'] == "전단출고실") {
            $query .= " AND dlvr_dvs = 'le' \n";
        }

        $query .= " ) as c \n";
        $rs = $conn->Execute($query);

        // bun_dlvr_order_num의 중복값으로 인해 데이터가 모두 검색되지 않을수 있다
        while($rs && !$rs->EOF) {
            return $rs->fields['cnt'];
            //$rs->MoveNext();
        }
    }

    function UpdateDlvrInfo($conn, $param) {
        $masks = array();
        if(array_key_exists('masks',$param))
            $masks = explode('|',$param['masks']);

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        var_dump($masks);
        $query = " UPDATE order_dlvr ";
        $query .= " SET tsrs_dvs = '송신' ";

        if(in_array("SENDERNAME", $masks)) {
            $query .= " , name = " . $param['SenderName'];
        }

        if(in_array("SENDERPHONE", $masks)) {
            $query .= " , tel_num = " . $param['SenderPhone'];
        }

        if(in_array("SENDERCELL", $masks)) {
            $query .= " , cell_num = " . $param['SenderCell'];
        }

        if(in_array("SENDERADDRESS", $masks)) {
            $query .= " , addr = " . $param['SenderAddress1'];
            $query .= " , addr_detail = " . $param['SenderAddress2'];
            $query .= " , zipcode = " . $param['SenderZipcode'];
        }

        $query .= " WHERE tsrs_dvs = '송신' ";
        $query .= " AND bun_dlvr_order_num = " . $param['bun_dlvr_order_num'];
        $query .= " AND bun_group = " . $param['bun_group'];

        $conn->Execute($query);

        $query = " UPDATE order_dlvr ";
        $query .= " SET tsrs_dvs = '수신' ";

        if(in_array("RECEIVERNAME", $masks)) {
            $query .= " , name = " . $param['ReceiverName'];
        }

        if(in_array("RECEIVERPHONE", $masks)) {
            $query .= " , tel_num = " . $param['ReceiverPhone'];
        }

        if(in_array("RECEIVERCELL", $masks)) {
            $query .= " , cell_num = " . $param['ReceiverCell'];
        }

        if(in_array("RECEIVERADDRESS", $masks)) {
            $query .= " , addr = " . $param['ReceiverAddress1'];
            $query .= " , addr_detail = " . $param['ReceiverAddress2'];
            $query .= " , zipcode = " . $param['ReceiverZipcode'];
        }

        if(in_array("DELIVERYWAY", $masks)) {
            $query .= " , dlvr_way = " . $param['SelectedDeliveryWay'];
        }

        if(in_array("RECEIVERADDRESS", $masks) || in_array("DELIVERYWAY", $masks)) {
            $query .= " , dlvr_price = " . $param['DlvrPrice'];
        }

        if(in_array("PAYWAY", $masks)) {
            $query .= " , dlvr_sum_way = " . $param['PayWay'];
        }

        $query .= " WHERE tsrs_dvs = '수신' ";
        $query .= " AND bun_dlvr_order_num = " . $param['bun_dlvr_order_num'];
        $query .= " AND bun_group = " . $param['bun_group'];

        $conn->Execute($query);
    }
}

?>
