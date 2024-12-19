<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 책자형클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 ***********************************************************************************/
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class Page extends Product implements PrintoutInterface
{
	/**
	 * @var string
	 */
	var $amt;
	var $size_w, $size_h;
	var $papers;
	var $options;
	var $afterprocesses;

	public function makeHtml() {
		$html = '<h2>페이지물 주문</h2></br></br>';
		$html .= '-----------표지-----------</br>';
		$html .= $this->makeBasicOption(0);

		$html .= '-----------내지1-----------</br>';
		$html .= $this->makeBasicOption(1);

		$html .= '-----------내지2-----------</br>';
		$html .= $this->makeBasicOption(2);

		$html .= $this->makeOptOption();
		$html .= $this->makeAfterOption();
		$html .= $this->makeCommonInfo();
		return $html;
	}

	function cost() {
		return 0;
	}

	function getSort() {
		return "leaflet";
	}
}

?>