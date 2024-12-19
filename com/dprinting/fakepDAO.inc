<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2018-01-24
 * Time: 오전 11:53
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');

class fakepDAO extends NimdaCommonDAO
{
    function __construct()
    {
    }

    public function selectProductList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    select MAX(A.order_detail_file_num) as order_detail_file_num, MAX(A.print_file_path) as print_file_path, ";
        $query .= "\n   MAX(A.print_file_name) as print_file_name, MAX(B.work_size_wid) as work_size_wid, MAX(B.amt) as amt, MAX(B.amt_unit_dvs) as amt_unit_dvs, ";
        $query .= "\n   MAX(E.oper_sys) as oper_sys, MAX(E.title) as title, MAX(E.order_detail) as order_detail, MAX(E.order_regi_date) as order_regi_date,  ";
        $query .= "\n   MAX(E.pay_price) as price, MAX(G.cate_name) as cate_name, MAX(B.tot_tmpt) as tot_tmpt, MAX(E.count) as count, MAX(E.order_mng) as order_mng, MAX(F.member_name) as send_name,";
        $query .= "\n   MAX(D.name) as receive_name, MAX(D.tel_num) as receive_telnum, MAX(D.cell_num) as receive_cellnum, MAX(D.addr) as receive_addr, MAX(D.addr_detail) as receive_addr_detail, MAX(D.dlvr_way) as dlvr_way, MAX(D.dlvr_sum_way) as dlvr_sum_way,  ";
        $query .= "\n   MAX(B.work_size_vert) as work_size_vert, MAX(B.cut_size_wid) as cut_size_wid, MAX(B.cut_size_vert) as cut_size_vert,  MAX(D.dlvr_way) as dlvr_way, MAX(D.dlvr_sum_way) as dlvr_sum_way, MAX(F.member_name) as member_name, ";
        $query .= "\n   MAX(C.preview_file_path) as preview_file_path, GROUP_CONCAT(C.preview_file_name Order by C.preview_file_name SEPARATOR '|') as preview_file_name ";
        $query .= "\n   from order_detail_count_file AS A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno  ";
        $query .= "\n   INNER JOIN order_detail_count_preview_file AS C ON A.order_detail_count_file_seqno = C.order_detail_count_file_seqno  ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON D.order_common_seqno = B.order_common_seqno AND D.tsrs_dvs = '수신'  ";
        $query .= "\n   INNER JOIN order_common AS E ON E.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   INNER JOIN member AS F ON F.member_seqno = E.member_seqno ";
        $query .= "\n   INNER JOIN cate AS G ON E.cate_sortcode = G.sortcode ";
        if($param['sortcode'] == "003001001" || $param['sortcode'] == "003001003")
            $query .= "\n   where B.cate_sortcode IN ('003001001','003001003') and E.amt = %s and B.tot_tmpt = %s ";
        else if($param['sortcode'] == "003001002" || $param['sortcode'] == "003001004")
            $query .= "\n   where B.cate_sortcode IN ('003001002','003001004') and E.amt = %s and B.tot_tmpt = %s ";
        else if(substr($param['sortcode'], 0, 6) === "003002")
            $query .= "\n   where B.cate_sortcode = '%s' and B.cate_paper_mpcode = %s and B.tot_tmpt = %s ";
        else
            $query .= "\n   where B.cate_sortcode = '%s' and B.cate_paper_mpcode = %s and E.amt = %s and B.tot_tmpt = %s ";
        $query .= "\n   AND A.state = '2120' AND C.order_detail_count_file_seqno is not null ";
        $query .= "\n   group by A.order_detail_file_num  ";

        if($param['sortcode'] == "003001001" || $param['sortcode'] == "003001002" || $param['sortcode'] == "003001003" || $param['sortcode'] == "003001004")
            $query = sprintf($query, $param['amt'], $param['side']);
        if($param['sortcode'] == "003002001")
            $query = sprintf($query, $param["sortcode"], $param["mpcode"], $param['side']);
        else
            $query = sprintf($query, $param["sortcode"], $param["mpcode"], $param['amt'], $param['side']);

        $rs = $conn->Execute($query);
        $products = array();
        while ($rs && !$rs->EOF) {
            $product = array();
            $product["order_detail_file_num"] = $rs->fields["order_detail_file_num"];
            $product["print_file_path"] = $rs->fields["print_file_path"];
            $product["print_file_name"] = $rs->fields["print_file_name"];
            $product["preview_file_path"] = $rs->fields["preview_file_path"];
            $product["preview_file_name"] = $rs->fields["preview_file_name"];
            $product["member_name"] = $rs->fields["member_name"];
            $product["work_size_wid"] = $rs->fields["work_size_wid"];
            $product["work_size_vert"] = $rs->fields["work_size_vert"];
            $product["cut_size_wid"] = $rs->fields["cut_size_wid"];
            $product["cut_size_vert"] = $rs->fields["cut_size_vert"];
            $product["amt"] = $rs->fields["amt"];
            $product["amt_unit_dvs"] = $rs->fields["amt_unit_dvs"];
            $product["dlvr_way"] = $rs->fields["dlvr_way"];
            $product["dlvr_sum_way"] = $rs->fields["dlvr_sum_way"];
            $product["filenames"] = $rs->fields["preview_file_name"];

            $product["oper_sys"] = $rs->fields["oper_sys"];
            $product["title"] = $rs->fields["title"];
            $product["order_detail"] = $rs->fields["order_detail"];
            $product["order_regi_date"] = $rs->fields["order_regi_date"];
            $product["price"] = $rs->fields["price"];
            $product["cate_name"] = $rs->fields["cate_name"];
            $product["tot_tmpt"] = $rs->fields["tot_tmpt"];
            $product["count"] = $rs->fields["count"];
            $product["receive_name"] = $rs->fields["receive_name"];
            $product["receive_telnum"] = $rs->fields["receive_telnum"];
            $product["receive_cellnum"] = $rs->fields["receive_cellnum"];
            $product["receive_addr"] = $rs->fields["receive_addr"];
            $product["receive_addr_detail"] = $rs->fields["receive_addr_detail"];
            $product["send_name"] = $rs->fields["send_name"];
            $product["dlvr_way"] = $rs->fields["dlvr_way"];
            $product["dlvr_sum_way"] = $rs->fields["dlvr_sum_way"];

            array_push($products, $product);
            $rs->MoveNext();
        }

        return $products;
    }

    public function selectAfterInfo() {

    }

    public function  selectOrderFile($conn, $param) {
        $query = "\n    select * ";
        $query .= "\n   from order_detail_count_preview_file ";
        $query .= "\n   WHERE preview_file_name = '%s' ";

        $query = sprintf($query, $param["order_detail_count_preview_file"]);

        $rs = $conn->Execute($query);
        $file_info = array();
        while ($rs && !$rs->EOF) {
            return $rs->fields["preview_file_path"] . "/" . $rs->fields["preview_file_name"];
        }

        return null;
    }

    public function selectPDFPath($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n   SELECT * ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num = %s ";

        $query = sprintf($query, $param["order_detail_file_num"]);
        $rs = $conn->Execute($query);
        $arr = array();
        while ($rs && !$rs->EOF) {
            return $rs->fields["print_file_path"] . "/" . $rs->fields["print_file_name"];
        }
    }

    public function selectAllPapers($conn) {
        $query = "\n   select concat(name, \" \", dvs, \" \", color, \" \", basisweight) as name, cate_paper_seqno  from cate_paper ";
        $query .= "\n  group by name, dvs, color, basisweight ";

        $rs = $conn->Execute($query);
        $names = array();
        while($rs && !$rs->EOF) {
            array_push($names, $rs->fields["name"]);
            $rs->MoveNext();
        }

        return $names;
    }

    public function selectPathList($conn, $param)
    {
        $query = "\n    select B.cate_sortcode, C.tot_name, C.cate_name, D.name, D.color, D.basisweight, D.mpcode, B.tot_tmpt,B.amt ,Count(*) as cnt  ";
        $query .= "\n   from order_detail_count_file AS A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN cate AS C ON B.cate_sortcode = C.sortcode ";
        $query .= "\n   INNER JOIN cate_paper AS D ON D.mpcode = B.cate_paper_mpcode ";
        $query .= "\n   where A.state = '2120' AND B.receipt_finish_date BETWEEN '%s' AND '%s 23:59:59' ";
        $query .= "\n   group by B.cate_sortcode, D.name, D.color, D.basisweight, D.mpcode, B.tot_tmpt, B.amt ";
        $query .= "\n   order by B.cate_sortcode ";

        $query = sprintf($query, $param['date'], $param['date']);
        $rs = $conn->Execute($query);
        $file_list = array();
        $file_list_count = array();
        $i = 0;
        while ($rs && !$rs->EOF) {
            // 코팅명함
            if ($rs->fields["cate_sortcode"] == "003001001" || $rs->fields["cate_sortcode"] == "003001003") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(명함)일반명함"]["코팅명함"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)일반명함";
                    $file_list[$i]["middle_category"] = "코팅명함";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)일반명함"]["코팅명함"][$small_cate] += $rs->fields["cnt"];
            } // 무코팅명함
            else if ($rs->fields["cate_sortcode"] == "003001002" || $rs->fields["cate_sortcode"] == "003001004") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(명함)일반명함"]["무코팅명함"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)일반명함";
                    $file_list[$i]["middle_category"] = "무코팅명함";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)일반명함"]["무코팅명함"][$small_cate] += $rs->fields["cnt"];
            } // 고급명함
            else if ($rs->fields["cate_sortcode"] == "003002001") {
                $nc_name = $rs->fields["name"] . " " . $rs->fields["color"] . " " . $rs->fields["basisweight"];
                $small_cate = $rs->fields["tot_tmpt"] . "_" . "200";

                if (!isset($file_list_count["(명함)고급명함"][$nc_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)고급명함";
                    $file_list[$i]["middle_category"] = $nc_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)고급명함"][$nc_name][$small_cate] += $rs->fields["cnt"];
            } // 강접스티커
            else if ($rs->fields["cate_sortcode"] == "004001001") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)컬러스티커"]["강접스티커"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)컬러스티커";
                    $file_list[$i]["middle_category"] = "강접스티커";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)컬러스티커"]["강접스티커"][$small_cate] += $rs->fields["cnt"];
            } // 특수지스티커
            else if ($rs->fields["cate_sortcode"] == "004002001") {
                $st_name = $rs->fields["name"] . "스티커";
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)특수지스티커"][$st_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)특수지스티커";
                    $file_list[$i]["middle_category"] = $st_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)특수지스티커"][$st_name][$small_cate] += $rs->fields["cnt"];
            } // 도무송스티커
            else if ($rs->fields["cate_sortcode"] == "004003001") {
                $st_name = $rs->fields["name"] . "스티커";
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)도무송스티커"][$st_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)도무송스티커";
                    $file_list[$i]["middle_category"] = $st_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)도무송스티커"][$st_name][$small_cate] += $rs->fields["cnt"];
            } // 합판전단지
            else if ($rs->fields["cate_sortcode"] == "005001001") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(전단)합판전단지"]["아트지 90g"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(전단)합판전단지";
                    $file_list[$i]["middle_category"] = "아트지 90g";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(전단)합판전단지"]["아트지 90g"][$small_cate] += $rs->fields["cnt"];
            }

            $rs->MoveNext();
        }

        $i = 0;
        $rs_arr = array();
        foreach ($file_list as $file) {
            //$file_list[$i]["count"] = $file_list_count[$file["large_category"]][$file["middle_category"]][$file["small_category"]];
            $afile = array();
            $afile["mpcode"] = $file["mpcode"];
            $afile["sortcode"] = $file["sortcode"];
            $afile["large_category"] = $file["large_category"];
            $afile["middle_category"] = $file["middle_category"];
            $afile["small_category"] = $file["small_category"];
            $afile["count"] = $file_list_count[$file["large_category"]][$file["middle_category"]][$file["small_category"]];
            array_push($rs_arr, $afile);
        }

        return $rs_arr;
    }

    function startsWith($haystack, $needle)
    {
        $length = strlen($needle);
        return (substr($haystack, 0, $length) === $needle);
    }
}
