
class Transaction {
	private String date;
	private Double payment;
	private String buyerName;
	
	Transaction(String date, Double payment, String buyerName) {
		this.date = date;
		this.payment = payment;
		this.buyerName = buyerName;
	}
	
	public String getDate() {
		// TODO Auto-generated method stub
		return date;
	}
	
	public Double getPayment() {
		// TODO Auto-generated method stub
		return payment;
	}
	
	public String getBuyerName() {
		// TODO Auto-generated method stub
		return buyerName;
	}
	
	public void setDate(String date) {
		this.date = date;
	}
	
	public void setPayment(Double payment) {
		this.payment = payment;
	}
	
	public void setbuyerName(String buyerName) {
		this.buyerName = buyerName;
	}
}
