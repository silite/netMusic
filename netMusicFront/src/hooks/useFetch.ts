export default async <T = unknown, R = unknown>(url: string, params: T) => {
  const res = await fetch(`http://localhost:10145/${url}`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(params),
  })

  return res.json() as Promise<R>
}
