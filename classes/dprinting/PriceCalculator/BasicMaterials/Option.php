<?
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/DAO/ProductDAO.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/CondimentDecorator.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/OptPriceUtil.inc");

class Option extends CondimentDecorator {
	var $product;
	var $option_name;
	var $price;
	var $depth1;
	var $depth2;
	var $depth3;
	var $mpcode;
	var $expect_box;


	var $dao;
	var $connectionPool;
	var $util;
	var $conn;

	function __construct($product)
	{
		$this->product = $product;
		$this->dao = new ProductDAO();
		$connectionPool = new ConnectionPool();
		$this->conn = $connectionPool->getPooledConnection();
	}

	function getDescription() {
		return $this->product->getDescription() . "옵션 : " . $this->option_name . "(" .$this->price . ")</br>";
	}

	function setOption($sortcode ,$arr_depth, $mpcode, $amt = '')
	{
		$this->option_name = $arr_depth["option_name"];
		$this->depth1 = $arr_depth["opt_depth1"];
		$this->depth2 = $arr_depth["opt_depth2"];
		$this->depth3 = $arr_depth["opt_depth3"];
		$this->opt_cnt = $arr_depth["opt_cnt"];
		$this->mpcode = $arr_depth["opt_mp_list"];

		if($arr_depth["option_name"] == "거치대") {
			$option_price = 0;
			$price_per = 0;
			switch($this->depth1) {
				case "실내용거치대 원스톱 배너" :
					$price_per = 7500 * 1.1;
					break;
				case "실내용거치대 포인트 배너" :
					$price_per = 7500 * 1.1;
					break;
				case "실외용거치대 샵플러스 배너 단면" :
					$price_per = 30000 * 1.1;
					break;
				case "실외용거치대 샵플러스 배너 양면" :
					$price_per = 31000 * 1.1;
					break;
			}
			$this->price = $price_per * $arr_depth["opt_cnt"];
		} else {
			$param = array();
			$param['cate_sortcode'] = $sortcode;
			$param['amt'] = $amt;
			$param['mpcode'] = $mpcode;
			$param['name'] = $arr_depth["opt_depth1"];
			$param['expect_box'] = $arr_depth["expect_box"];
			$param['affil'] = $arr_depth["affil"];
			$param['paper_mpcode'] = $arr_depth["paper_mpcode"];
			$param['paper_info'] = $arr_depth["paper_info"];
			$optUtil = new OptPriceUtil($param);
			$rs_price  = $optUtil->calcOptPrice($param);
			$this->setPrice($rs_price);
		}
	}

	function setPrice($rs_price) {
		$this->price = $rs_price['sell_price'];
	}

	function cost() {
		$result = $this->product->cost();
		$result[$this->option_name] = $this->price;
		$result["add_option_price"] = $result["add_option_price"] + $this->price;
		$result["sell_price"] = $result["sell_price"] + $this->price;

		if(array_key_exists("빠른출고", $result)) {
			//$product = $this->getProduct();
			//var_dump($result["sell_price"]);
			$extra_price = 0;
			if($this->depth1 == "오늘출고") $extra_price = round($result["sell_price"] * 0.2 * 0.01) * 100;
			if($this->depth1 == "내일출고") $extra_price = round($result["sell_price"] * 0.15 * 0.01) * 100;

			//$result["sell_price"] += $extra_paper_price;
			$result["빠른출고"] = $extra_price;
			$result["add_option_price"] = $result["add_option_price"] + $extra_price;
			$result["sell_price"] = $result["sell_price"] + $extra_price;
		}

		if(array_key_exists("정매생산요청", $result)) {
			$extra_paper_price = round($result["paper"] * 0.1 * 0.01) * 100;
			$result["sell_price"] += $extra_paper_price;
			$result["정매생산요청"] = $extra_paper_price;
		}

		if(array_key_exists("재단", $result)) {
			$product = $this->getProduct();
			//var_dump($product);
			switch ($product->size_name) {
				case "A1":
					$pos = 500;
					break;
				case "국3절":
					$pos = 750;
					break;
				case "A2":
				case "2절(B2)":
				case "국장2절":
					$pos = 1000;
					break;
				case "A3":
				case "4절(B3)":
					$pos = 2000;
					break;
				case "A4":
				case "8절(B4)":
					$pos = 4000;
					break;
				case "A5":
				case "16절(B5)":
					$pos = 8000;
					break;
				case "A6":
				case "32절(B6)":
					$pos = 16000;
					break;
				case "64절(B7)":
					$pos = 32000;
					break;
			}

			$r = $product->amt / $pos;
			$price = 0;
			switch ($this->depth1) {
				case "2등분":
					$price = 2000;
					break;
				case "3등분":
					$price = 3000;
					break;
				case "4등분":
					$price = 4000;
					break;
				case "5등분":
					$price = 5000;
					break;
				case "6등분":
					$price = 7000;
					break;
				case "7등분":
					$price = 8000;
					break;
				case "8등분":
					$price = 10000;
					break;
				case "9등분":
					$price = 12000;
					break;
				case "10등분 이상":
					$price = 15000;
					break;
			}
			if($r < 1) $r = 1;
			$price *= $r;
			$result[$this->option_name] = $price;
			$result["add_option_price"] = $result["add_option_price"] + $price;
			$result["sell_price"] = $result["sell_price"] + $price;
		}

		return $result;
	}

	function getAfterList() {
		return $this->product->getAfterList();
	}

	function getOptionList() {
		$p = $this->getProduct();
		if($p->order_option_history_param == null) {
			$p->order_option_history_param = array();
		}
		//$ret["opt_eng_name"] = $this->option_name;
		$ret["mpcode"]     = $this->mpcode;
		$ret["opt_name"] = $this->option_name;
		$ret["depth1"]     = $this->depth1;
		$ret["depth2"]     = "-";
		$ret["depth3"]     = "-";
		$ret["basic_yn"]     = "N";
		$ret["price"]     = $this->price;
		return $ret;

	}

	function getProduct() {
		return $this->product->getProduct();
	}

	function makeOrderCommonInsertParam($fb, $session) {
		return $this->product->makeOrderCommonInsertParam($fb, $session);
	}

	function makeOrderDetailInsertParam($fb, $session) {
		return $this->product->makeOrderDetailInsertParam($fb, $session);
	}

	function makeOrderDlvrInsertParam($fb) {
		return $this->product->makeOrderDlvrInsertParam($fb);
	}

	function makeOrderAfterHistoryInsertParam($fb) {
		return $this->product->makeOrderAfterHistoryInsertParam($fb);
	}

	function makeOrderOptionHistoryInsertParam($fb) {
		$param = $this->getOptionList();
		$p = $this->getProduct();
		if($p->order_option_history_param == null)
			$p->order_option_history_param = array();
		array_push($p->order_option_history_param, $param);

		return $this->product->makeOrderOptionHistoryInsertParam($fb);
	}

	function makeOrderFileInsertParam($fb) {
		return $this->product->makeOrderFileInsertParam($fb);
	}
}

?>
