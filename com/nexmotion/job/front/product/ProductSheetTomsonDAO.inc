<?
include_once(INC_PATH . '/common_dao/ProductCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/product/TomsonCommonHtml.inc');

class ProductSheetTomsonDAO extends ProductCommonDAO {
    function __construct() {
    }

    /**
     * @brief 도무송 사이즈 종류 검색
     *
     * @param $conn           = connection identifier
     * @param $cate_sortcode  = 카테고리 분류코드
     * @param $price_info_arr = 가격검색용 정보저장 배열
     *
     * @return option html
     */
    function selectTomsonSizeTyp($conn,
                                 $cate_sortcode,
                                 &$price_info_arr,
                                 $affil_yn,
                                 $size_typ_yn,
                                 $order_size) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $default = ProductDefaultSel::DEFAULT_SEL[$cate_sortcode]["size_typ"];
        if (empty($default) === true) {
            $default = true;
        }
        $price_info_arr["size_typ"] = $default;

        $cate_sortcode = $this->parameterEscape($conn, $cate_sortcode);

        $query  = "\n SELECT  A.typ AS name";
        $query .= "\n        ,A.work_wid_size";
        $query .= "\n        ,A.work_vert_size";
        $query .= "\n        ,A.cut_wid_size";
        $query .= "\n        ,A.cut_vert_size";
        $query .= "\n        ,A.tomson_wid_size";
        $query .= "\n        ,A.tomson_vert_size";
        $query .= "\n        ,A.design_wid_size";
        $query .= "\n        ,A.design_vert_size";
        $query .= "\n        ,B.mpcode";

        $query .= "\n   FROM  prdt_stan AS A";
        $query .= "\n        ,cate_stan AS B";

        $query .= "\n  WHERE  A.prdt_stan_seqno = B.prdt_stan_seqno";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "cate_mpcode")) {
            $query .= "\n    AND  B.mpcode IN (";
            $query .= $param["cate_mpcode"];
            $query .= ')';
        }

        // 규격도무송 숨김
        if($cate_sortcode != "'004004001'")
            $query .= "\n  AND B.mpcode < 631 ";

        $query .= "\n ORDER BY B.seq, typ ";
        $query  = sprintf($query, $cate_sortcode);
        $rs = $conn->Execute($query);

        if ($order_size) {
            $default = $order_size;
        }

        return makeCateSizeOption($rs,
                                  $default,
                                  null,
                                  $price_info_arr,
                                  $affil_yn,
                                  $size_typ_yn);
    }
}
?>

