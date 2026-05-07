import csv
import random

# Các thuộc tính mẫu dựa trên đặc tả của lab 
SELLERS = ['A', 'B', 'C', 'D', 'E']
JOB_TYPES = ['resize', 'thumbnail', 'compress', 'policy_check', 'metadata_update']
HEADERS = ['job_id', 'seller_id', 'arrival_time', 'estimated_runtime', 'priority', 'job_type'] # [cite: 52]

def write_csv(filename, jobs):
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerow(HEADERS)
        for job in jobs:
            writer.writerow(job)
    print(f"[+] Created {filename} with {len(jobs)} jobs.")

def generate_workload_a():
    """Workload A: Balanced Workload - Runtime và Priority hỗn hợp [cite: 67, 68]"""
    jobs = []
    for i in range(1, 21):
        job_id = i
        seller = random.choice(SELLERS)
        arrival = i - 1  # Đến đều đặn từng giây
        runtime = random.randint(3, 6) # Runtime tương đồng nhau
        priority = random.randint(1, 3)
        job_type = random.choice(JOB_TYPES)
        jobs.append([job_id, seller, arrival, runtime, priority, job_type])
    write_csv('workloads/workload_a.csv', jobs)

def generate_workload_b():
    """Workload B: Mixed Short and Long Jobs - Kích hoạt Convoy Effect [cite: 70, 71]"""
    jobs = []
    # Job đầu tiên cực kỳ dài để làm nghẽn hàng đợi FIFO
    jobs.append([1, 'A', 0, 25, 2, 'compress']) 
    
    # Các job sau rất ngắn, bị kẹt lại phía sau job 1
    for i in range(2, 21):
        seller = random.choice(SELLERS)
        arrival = i  
        runtime = random.randint(1, 2) # Rất ngắn
        priority = 2
        job_type = 'thumbnail'
        jobs.append([i, seller, arrival, runtime, priority, job_type])
    write_csv('workloads/workload_b.csv', jobs)

def generate_workload_c():
    """Workload C: Priority-Sensitive Workload - Kiểm tra Starvation [cite: 72, 73]"""
    jobs = []
    # 10 jobs đầu tiên đến sớm nhưng độ ưu tiên THẤP (số 3, 4)
    for i in range(1, 11):
        seller = random.choice(SELLERS)
        arrival = i - 1
        runtime = random.randint(4, 7)
        priority = random.choice([3, 4]) 
        job_type = random.choice(JOB_TYPES)
        jobs.append([i, seller, arrival, runtime, priority, job_type])
        
    # 10 jobs sau đến trễ nhưng độ ưu tiên CAO (số 1)
    for i in range(11, 21):
        seller = random.choice(SELLERS)
        arrival = random.randint(5, 12) # Đến chèn vào giữa lúc các job thấp đang chờ
        runtime = random.randint(2, 4)
        priority = 1 # Ưu tiên cao nhất
        job_type = 'policy_check'
        jobs.append([i, seller, arrival, runtime, priority, job_type])
        
    # Sắp xếp lại theo arrival_time để mô phỏng luồng thời gian tuyến tính
    jobs.sort(key=lambda x: x[2])
    write_csv('workloads/workload_c.csv', jobs)

if __name__ == "__main__":
    print("Generating workloads...")
    generate_workload_a()
    generate_workload_b()
    generate_workload_c()
    print("Done!")