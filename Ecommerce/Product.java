
class Product {
	private String name;
	private String id;
	private Double price;
	
	Product(String name, String id, Double price) {
		this.name = name;
		this.id = id;
		this.price = price;
	}
	
	public String getId() {
		return id;
	}
	
	public String getName() {
		return name;
	}
	
	public double getPrice() {
		// TODO Auto-generated method stub
		return price;
	}
	
	public void setPrice(double price) {
		// TODO Auto-generated method stub
		this.price = price;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public void setName(String name) {
		this.name = name;
	}
}
